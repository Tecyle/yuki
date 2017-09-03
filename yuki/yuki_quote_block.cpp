#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_quote_block.h"

/*
	解析要点：

	1. 跳过空行
	2. 跳过 ``--`` 打头的文字段
	3. 按照 inline_block 解析
*/
bool YukiQuoteBlockAttribute::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	if (!matchNoBackward())
	{
		reader->setCursor(oldCursor);
		return false;
	}

	YukiQuoteBlockAttributeNode* attrNode = new YukiQuoteBlockAttributeNode;
	getParser(L"inline_block")->parse(attrNode, reader->cutRegionFromCursorToEnd());
	parentNode->appendChild(parentNode);
	return true;
}

/*
	在 match 的时候，仅判断前缀，不判断缩进相关
	缩进相关的应该在 quote block 那一层判断
*/
bool YukiQuoteBlockAttribute::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiQuoteBlockAttribute::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();

	reader->skipSpaces();
	if (!reader->matchStr(L"---")
		&& !reader->matchStr(L"--")
		&& !reader->matchStr(L"——"))
	{
		return false;
	}

	if (reader->skipSpaces() <= 0)
		return false;

	if (reader->cursorAtLineEnd())
		return false;

	return true;
}
