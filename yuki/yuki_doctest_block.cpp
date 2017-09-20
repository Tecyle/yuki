#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_doctest_block.h"

bool YukiDoctest::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	const YukiRegion* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!match())
		goto match_finished;

	YukiDoctestNode* node = new YukiDoctestNode;
	yuki_cursor oldCursor = reader->getCursor();
	yuki_cursor newCursor = reader->getCursor();
	// ���������
	while (reader->moveToNextLine())
	{
		newCursor = reader->getCursor();
		if(reader->matchBlankLine())
			break;
		if (!reader->matchStr(L"...") || reader->skipSpaces() < 1)
			break;
	}
	reader->setCursor(oldCursor);
	getParser(L"reserved_text")->parse(node, reader->cutRegionBetween(oldCursor, newCursor, 3));
	// �����������
	oldCursor = reader->getCursor();
	while (!reader->matchBlankLine())
	{
		if (!reader->moveToNextLine())
			break;
	}
	newCursor = reader->getCursor();
	reader->setCursor(oldCursor);
	getParser(L"reserved_text")->parse(node, reader->cutRegionBetween(oldCursor, newCursor, 0));
	succ = true;

	parentNode->appendChild(node);
match_finished:
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiDoctest::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiDoctest::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();
	
	if (!reader->matchStr(L">>>"))
		return false;
	if (reader->skipSpaces() < 1)
		return false;

	return true;
}

