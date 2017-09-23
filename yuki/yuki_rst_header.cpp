#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_region_ex.h"
#include "yuki_rst_header.h"

bool YukiRstHeader::parse(YukiNode* parentNode, const yuki_region* region)
{
	if (!match())
		return false;

	YukiFileReader* reader = getFileReader();
	YukiRstHeaderNode* headerNode = new YukiRstHeaderNode;
	wchar_t upperChar = 0;
	wchar_t lowerChar = 0;
	int upperSize = 0;
	int lowerSize = 0;
	int titleSize = 0;
	// 上标线判断，上标线仅做装饰，如果上标线和下标线符号不一致，仅给出警告，以下标线为准
	const YukiLineString* line = reader->getLine();
	if (line->isSeperator())
	{
		// 约定，标题线必须没有缩进
		upperChar = line->getCharAtIndex(0);
		upperSize = line->getContentLength();
		reader->moveToNextLine();
		line = reader->getLine();
	}

	// 标题内容解析
	titleSize = line->getIndent() + line->getContentLength();
	const yuki_region* headerTextRegion = new YukiLinedRegion(line->beginCursor(), line->endCursor());
	getParser(L"inline_block")->parse(headerNode, headerTextRegion);

	// 下标线分析
	reader->moveToNextLine();
	line = reader->getLine();
	assert(line->isSeperator());
	lowerChar = line->getCharAtIndex(0);
	lowerSize = line->getContentLength();
	headerNode->setHeaderChar(lowerChar);
	headerNode->setDoubleLined(upperChar != 0);

	if (upperChar != 0 && upperChar != lowerChar)
	{
		if (upperChar != lowerChar)
		{
			// @WARNING 上下标线所用的字符不一致
		}
		if (upperSize != lowerSize)
		{
			// @WARNING 上下标线的长度不一致
		}
	}

	if (lowerSize < titleSize)
	{
		// @WARNING 下标线的长度少于标题的长度
	}

	reader->moveToNextLine();
	parentNode->appendChild(headerNode);
	return true;
}

/*
	rst 风格的标题识别原则：

	1. 行首没有缩进
	2. 第一行是分隔线或者正常内容
	3. 第一行如果是分隔线，则第二行是正常内容
	4. 正常内容后面那行应该是分隔线
*/
bool YukiRstHeader::match()
{
	YukiFileReader* reader = getFileReader();
	const YukiLineString* line = reader->getLine();

	if (line == nullptr || line->isBlankLine() || line->getIndent() != 0)
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
