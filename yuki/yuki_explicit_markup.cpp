#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_explicit_markup.h"

bool YukiExplicitMarkup::parse(YukiNode* parentNode, const yuki_region* region)
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
	bool meetContent = !reader->cursorAtLineEnd();
	bool lastLineIsBlankLine = false;
	int commonIndent = INT_MAX;
	yuki_cursor endCursor;
	yuki_cursor startCursor = oldCursor;
	oldCursor = reader->getCursor();
	while (reader->moveToNextLine())
	{
		const yuki_line_string* line = reader->getLine();

		if (line == nullptr)
			break;

		if (line->isBlankLine())
		{
			// 用 endCursor 去掉末尾多余的空行
			if (meetContent && !lastLineIsBlankLine)
				endCursor = reader->getCursor();
			lastLineIsBlankLine = true;
			continue;
		}

		if (line->getIndent() <= indent)
			break;

		if (lastLineIsBlankLine)
		{
			// 在遇见正文内容之前遇到的空行全部忽略
			if (!meetContent)
			{
				startCursor = reader->getCursor();
				meetContent = true;
			}
			lastLineIsBlankLine = false;
		}
		commonIndent = yuki_min(commonIndent, line->getIndent());
	}
	const yuki_region* explicitBlockRegion = meetContent ?
		reader->cutRegionBetween(startCursor, endCursor, commonIndent)
		: reader->cutRegionToCursorFrom(oldCursor);
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
