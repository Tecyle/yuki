#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_matcher.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_rst_title_matcher.h"

/*
	rst 风格的标题识别原则：

	1. 行首没有缩进
	2. 第一行是分隔线或者正常内容
	3. 第一行如果是分隔线，则第二行是正常内容
	4. 正常内容后面那行应该是分隔线
*/
bool YukiRstTitleMatcher::lookAhead(const YukiLineString* line, YukiFileReader* reader)
{
	if (line->isBlankLine() || line->getIndent() != 0)
		return false;

	yuki_cursor oldCursor = reader->getCursor();
	bool succ = false;

	// 第一行如果是分隔线，就下移一行
	if (line->isSeperator())
	{
		if (!reader->moveToNextLine())
			goto match_failed;
		line = reader->getLine();
	}

	// 第二行不能是空行，也不能是分隔线，但是可以有缩进
	if (line->isBlankLine() || line->isSeperator())
		goto match_failed;

	// 下移一行，后面一行必须是分隔线
	if (!reader->moveToNextLine())
		goto match_failed;
	line = reader->getLine();

	succ = line->isSeperator();
	
match_failed:
	reader->setCursor(oldCursor);
	return succ;
}
