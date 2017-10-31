#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_md_literal_block.h"
#include "yuki_literal_block_node.h"

static bool yuki_isLanguageNameChar(wchar_t ch)
{
	return !isspace(ch) && ch != '`';
}

bool yuki_md_literal_block::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	reader->pushRegion(region);
	bool succ = false;

	if (!matchNoBackward())
		goto match_finished;

	oldCursor = reader->getCursor();
	yuki_cursor newCursor = reader->getCursor();
	const yuki_line_string* line = reader->getLine();
	// ���������λ��
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
	// �������岿λ��λ��
	const yuki_region* bodyRegion = reader->cutRegionBetween(oldCursor, newCursor);
	yuki_literal_block_node* node = new yuki_literal_block_node;
	node->setLanguage(m_language);
	reader->setCursor(oldCursor);
	getParser(L"reserved_text")->parse(node, bodyRegion);
	// ������֮�󣬹��ͣ�� ``` ��һ�У������ļ�ĩβ����������һ��
	reader->moveToNextLine();

	parentNode->appendChild(node);
	succ = true;

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->popRegion();
	return succ;
}

bool yuki_md_literal_block::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool yuki_md_literal_block::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();

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
	// ���ͬһ�д��� ```����ʶ��ÿ�
	if (reader->searchStr(L"```"))
		return false;

	reader->moveToNextLine();
	return true;
}
