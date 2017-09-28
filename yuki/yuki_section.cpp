#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_section.h"

/*
	章节的解析规则：

	1. 先匹配一个标题，如果匹配失败，则认为这是没有标题的章节
	2. 再向下搜索下一个标题，中间的部分就是章节体

	标题的前面必须有空行或者是文档开头
*/
bool YukiSection::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* fileReader = getFileReader();
	const yuki_region* oldRegion = fileReader->selectRegion(region);
	YukiSectionNode* sectionNode = new YukiSectionNode;

	fileReader->skipBlankLines();
	if (!getParser(L"md_header")->parse(sectionNode, region)
		&& !getParser(L"rst_header")->parse(sectionNode, region))
	{
		// Section with no header
	}

	getParser(L"body")->parse(sectionNode, searchSectionBodyRegion());

	parentNode->appendChild(sectionNode);
	fileReader->selectRegion(oldRegion);
	return true;
}

const yuki_region* YukiSection::searchSectionBodyRegion()
{
	yuki_file_reader* fileReader = getFileReader();
	bool lastLineIsBlankLine = false;

	yuki_cursor oldCursor = fileReader->getCursor();
	do
	{
		const yuki_line_string* line = fileReader->getLine();

		if (line->isBlankLine())
		{
			lastLineIsBlankLine = true;
			continue;
		}

		if (lastLineIsBlankLine)
		{
			if (getParser(L"md_header")->match()
				|| getParser(L"rst_header")->match())
			{
				yuki_cursor endCursor = fileReader->getCursor();
				fileReader->setCursor(oldCursor);
				return fileReader->cutRegionFromCursorTo(endCursor);
			}
			lastLineIsBlankLine = false;
		}
	} while (fileReader->moveToNextLine());

	fileReader->setCursor(oldCursor);
	return fileReader->getRegion();
}
