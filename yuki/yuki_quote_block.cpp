#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_quote_block.h"

/*
	����Ҫ�㣺

	1. ��������
	2. ���� ``--`` ��ͷ�����ֶ�
	3. ���� inline_block ����
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
	�� match ��ʱ�򣬽��ж�ǰ׺�����ж��������
	������ص�Ӧ���� quote block ��һ���ж�
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
		&& !reader->matchStr(L"����"))
	{
		return false;
	}

	if (reader->skipSpaces() <= 0)
		return false;

	if (reader->cursorAtLineEnd())
		return false;

	return true;
}
