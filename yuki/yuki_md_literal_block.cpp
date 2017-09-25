#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_md_literal_block.h"

static bool yuki_isLanguageNameChar(wchar_t ch)
{
	return !isspace(ch) && ch != '`';
}

bool YukiMdLiteralBlock::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!matchNoBackward())
		goto match_finished;

	oldCursor = reader->getCursor();
	yuki_cursor newCursor = reader->getCursor();
	const yuki_line_string* line = reader->getLine();
	// 搜索块结束位置
	while (line != nullptr)
	{
		newCursor = reader->getCursor();
		if (reader->matchStr(L"```"))
		{
			reader->skipSpaces();
			if (reader->cursorAtLineEnd())
				break;
		}
		reader->moveToNextLine();
		line = reader->getLine();
	}
	// 决定主体部位的位置
	const yuki_region* bodyRegion = reader->cutRegionBetween(oldCursor, newCursor);
	YukiLiteralBlockNode* node = new YukiLiteralBlockNode;
	node->setLanguage(m_language);
	reader->setCursor(oldCursor);
	getParser(L"reserved_text")->parse(node, bodyRegion);
	// 解析完之后，光标停在 ``` 这一行，或者文件末尾，尝试下移一行
	reader->moveToNextLine();

	parentNode->appendChild(node);
	succ = true;

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiMdLiteralBlock::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiMdLiteralBlock::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();

	if (!reader->matchStr(L"```"))
		return false;

	wchar_t ch = reader->getChar();
	m_language.clear();
	while (yuki_isLanguageNameChar(ch))
	{
		m_language += ch;
		if (!reader->moveToNextChar(false))
			break;
		ch = reader->getChar();
	}
	// 如果同一行存在 ```，则不识别该块
	if (reader->searchStr(L"```"))
		return false;

	reader->moveToNextLine();
	return true;
}
