#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_structure_parser.h"
#include "yuki_explicit_markup.h"

bool yuki_explicit_markup::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
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
		if (getParser(yuki_footnote_citation_definition_block_name)->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	case '_':
		if (getParser(yuki_hyperlink_target_block_name)->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	case '|':
		if (getParser(yuki_substitution_definition_block_name)->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	case 0:
	case '\n':
		break;
	default:
		if (getParser(yuki_directives_block_name)->parse(parentNode, explicitBlockRegion))
			return true;
		break;
	}

	return getParser(yuki_comment_block_name)->parse(parentNode, explicitBlockRegion);
}

bool yuki_explicit_markup::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	bool succ = matchNoBackward();
	reader->setCursor(oldCursor);
	return succ;
}

bool yuki_explicit_markup::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();
	bool succ = false;

	if (!reader->matchStr(L".."))
		return false;

	if (!reader->cursorAtLineEnd() && reader->skipSpaces() == 0)
		return false;

	return true;
}
