#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_node.h"
#include "yuki_struct.h"
#include "yuki_file_reader.h"
#include "yuki_region_ex.h"
#include "yuki_line_string.h"
#include "yuki_md_header.h"

/*
*/
bool YukiMdHeader::parse(YukiNode* parentNode, const yuki_region* region)
{
	if (!match())
		return false;

	YukiFileReader* reader = getFileReader();
	YukiMdHeaderNode* headerNode = new YukiMdHeaderNode;
	int headerLevel = 0;
	while (reader->matchChar('#'))
	{
		headerLevel++;
	}
	headerNode->setHeaderLevel(headerLevel);
	reader->skipSpaces();

	// 检查标题行的末尾是否有对称的标题标记符号
	yuki_cursor startCursor = reader->getCursor();
	yuki_cursor endCursor = reader->findSuffixChar('#', headerLevel);
	const yuki_region* headerTextRegion = new YukiLinedRegion(startCursor, endCursor);

	getParser(L"inline_block")->parse(headerNode, headerTextRegion);
	reader->moveToNextLine();
	parentNode->appendChild(headerNode);

	return true;
}

/*
	Markdown 标题的确认标志

	1. 判断行不能有缩进
	2. 以若干个 ``#`` 打头，并且 ``#`` 后面有至少一个空格
	3. ``#`` 后面至少有一个非空白字符
*/
bool YukiMdHeader::match()
{
	bool succ = false;
	YukiFileReader* reader = getFileReader();
	const YukiLineString* line = reader->getLine();
	if (line == nullptr)
		return false;

	if (line->isBlankLine() || line->getIndent() != 0)
		return false;

	if (!reader->matchChar('#'))
		return false;

	yuki_cursor oldCursor = reader->getCursor();
	while (reader->matchChar('#'));

	if (reader->skipSpaces() == 0)
		goto match_failed;

	if (reader->cursorAtLineEnd())
		goto match_failed;

	succ = true;

match_failed:
	reader->setCursor(oldCursor);
	return succ;
}
