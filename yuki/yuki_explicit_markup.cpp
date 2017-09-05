#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_explicit_markup.h"

bool YukiExplicitMarkup::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	if (!matchNoBackward())
	{
		reader->setCursor(oldCursor);
		return false;
	}
	// 搜索块范围
	int indent = reader->getLine()->getIndent();
	oldCursor = reader->getCursor();
	while (reader->moveToNextLine())
	{
		const YukiLineString* line = reader->getLine();

		if (line == nullptr)
			break;

		if(line->isBlankLine())
			continue;

		if (line->getIndent() <= indent)
			break;
	}
	const YukiRegion* explicitBlockRegion = reader->cutRegionToCursorFrom(oldCursor);
	reader->setCursor(oldCursor);
	// 决定使用什么块解析
	wchar_t ch = reader->getChar();
	switch (ch)
	{
	case '[':
		if (getParser(L"footnote_citation_definition")->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	case '_':
		if (getParser(L"hyperlink_target")->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	case '|':
		if (getParser(L"substitution_definition")->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	case 0:
	case '\n':
		break;
	default:
		if (getParser(L"directive")->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	}

	return getParser(L"comment")->parse(parentNode, explicitBlockRegion);
}

bool YukiExplicitMarkup::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	bool succ = matchNoBackward();
	reader->setCursor(oldCursor);
	return succ;
}

bool YukiExplicitMarkup::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();
	bool succ = false;

	if (!reader->matchStr(L".."))
		return false;

	if (!reader->cursorAtLineEnd() && reader->skipSpaces() == 0)
		return false;

	return true;
}
