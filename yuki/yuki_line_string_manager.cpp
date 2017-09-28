#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_line_string.h"
#include "yuki_file_string.h"
#include "yuki_object_allocator.h"
#include "yuki_line_string_manager.h"

typedef yuki_object_allocator<yuki_line_string, yuki_file_string*> yuki_line_string_allocator;

yuki_line_string_manager::yuki_line_string_manager()
	: m_allocator(new yuki_line_string_allocator())
{
}

yuki_line_string_manager::~yuki_line_string_manager()
{
	delete m_allocator;
}

/*
	构造一个 line string，只需要确定四个 cursor 的位置即可。
*/
const yuki_line_string* yuki_line_string_manager::allocLineStringForFileString(
	yuki_file_string* fileString, int ln, wchar_t* & str)
{
	assert(ln < fileString->getLineCount());

	yuki_line_string* line = allocLineString(fileString);
	yuki_cursor& cursor = line->m_lineHeadCursor;
	cursor.setCursor(ln, 0, str - fileString->getBuffer(), 0);

	wchar_t* beginPos = str;
	bool needPassOneChar = false;
	bool countingIndent = true;
	// 计算缩进，得出内容起点游标
	wchar_t ch = *str;
	str++;
	int col = 0;
	int chCount = 0;
	while (!yukiIsLineBreak(ch) && isspace(ch))
	{
		col = ch == '\t' ? yukiAlignPosToTabSize(col) : col + 1;
		chCount++;
		ch = *str;
		str++;
	}
	// 这一行只有空白字符，说明是空行
	if (yukiIsLineBreak(ch) || ch == 0)
	{
		line->m_lineTailCursor = line->m_lineHeadCursor;
		line->m_contentHeadCursor = line->m_lineHeadCursor;
		line->m_contentTailCursor = line->m_contentHeadCursor;
		return line;
	}

	cursor = line->m_contentHeadCursor;
	cursor.setCursor(ln, col, str - fileString->getBuffer(), chCount);

	// 计算主体内容长度
	cursor = line->m_contentTailCursor;
	cursor.setCursor(ln, col, str - fileString->getBuffer(), chCount);
	bool isLastCharSpace = false;
	// 下面的循环肯定至少会执行一次
	for (; ch != 0 && !yukiIsLineBreak(ch); str++)
	{
		ch = *str;
		// 遇到空白字符，记录位置，此处可能是行尾空白
		if (!isLastCharSpace && isspace(ch))
		{
			cursor.col = col;
			cursor.offset = str - fileString->getBuffer();
			cursor.ch = chCount;
			isLastCharSpace = true;
		}

		col = ch == '\t' ? yukiAlignPosToTabSize(col) : col + 1;
		chCount++;
	}
	// 判断内容终点游标是否正确设置
	if (!isLastCharSpace)
	{
		cursor.col = col;
		cursor.offset = str - fileString->getBuffer();
		cursor.ch = chCount;
	}

	// 设置终点游标
	cursor = line->m_lineTailCursor;
	cursor.col = col;
	cursor.offset = str - fileString->getBuffer();
	cursor.ch = chCount;
	if (str[-1] == '\r')
	{
		cursor.col--;
		cursor.offset--;
		cursor.ch--;
	}

	str++;
	return line;
}

/*
	裁剪 line string 的逻辑：
	
	1. region 如果是行连续区域：

		1. 第二行开始，所有行，如果原始缩进大于 region 的缩进，则使用原始
		   缩进，否则，从 region 的缩进位置开始，跳过空白字符增加缩进。
		2. 行尾和原始行行尾一致，裁剪至终点游标。
		3. 如果是首行，如果起点位置小于等于 region 的缩进，则和其它行保持
		   一样的逻辑，如果起点位置大于 region 的缩进，则采用 region 的缩
		   进。

	2. region 如果是块连续区域：

		1. 第二行开始，所有行截取 leftEdge 到 rightEdge 之间的内容，如果原
		   始缩进大于 region 的缩进 + leftEdge 的列号，则使用原始缩进，否则
		   从那个位置开始跳过空白字符增加缩进。
		2. 如果是首行，处理方法和行连续区域一致，只是需要限定在 leftEdge 和
		   rightEdge 之间。
		3. 如果是尾行，裁剪至终点游标。
*/
const yuki_line_string* yuki_line_string_manager::allocLineStringForRegionString(yuki_file_string* fileString, 
	const yuki_region* region, int lnInRegion)
{
	// 检查行号是否越界
	const yuki_cursor& endPos = region->end();
	// 如果 endCursor 定位在某行的行首，应该不包括这一行，上层逻辑保证这点
	if (endPos.isValid() && lnInRegion > endPos.ln)
		return nullptr;
	if (!endPos.isValid() && lnInRegion >= fileString->getLineCount())
		return nullptr;

	// 开始构造 line string
	yuki_line_string* line = allocLineString(fileString);
	const yuki_line_string* originalLine = fileString->getLine(region->begin().ln + lnInRegion);
	assert(originalLine);

	if (lnInRegion == region->begin().ln)
	{
		// 处理首行
		line->m_lineHeadCursor = region->begin();
		if (region->getRegionType() == Yuki_linedRegion)
		{
			line->m_lineTailCursor = originalLine->m_lineTailCursor;
		}
		else
		{
			line->m_lineTailCursor = region->getRightEdige();
		}
	}
	else if ((!region->end().isValid() && lnInRegion < fileString->getLineCount())
		|| lnInRegion < region->end().ln)
	{
		// 处理中间行
		if (region->getRegionType() == Yuki_linedRegion)
		{
			line->m_lineHeadCursor = originalLine->m_lineHeadCursor;
			line->m_lineTailCursor = originalLine->m_lineTailCursor;
		}
		else
		{
			line->m_lineHeadCursor = region->getLeftEdgeCursor();
			line->m_lineTailCursor = region->getRightEdige();
		}
	}
	else
	{
		// 处理尾行
		line->m_lineTailCursor = region->end();
		if (region->getRegionType() == Yuki_linedRegion)
		{
			line->m_lineHeadCursor = originalLine->m_lineHeadCursor;
		}
		else
		{
			line->m_lineHeadCursor = region->getLeftEdgeCursor();
		}
	}
	// content tail
	if (line->m_lineTailCursor < originalLine->m_contentTailCursor)
		line->m_contentTailCursor = line->m_lineTailCursor;
	else
		line->m_contentTailCursor = originalLine->m_contentTailCursor;
	// content head
	if (originalLine->getIndent() >= region->getIndent())
	{
		line->m_contentHeadCursor = originalLine->m_contentHeadCursor;
	}
	else
	{
		yuki_cursor& cursor = line->m_contentHeadCursor;
		cursor = originalLine->getCursorByCol(region->getIndent());
		wchar_t ch = originalLine->getCharAtCursor(cursor);
		while (isspace(ch))
		{
			cursor.col = ch == '\t' ? yukiAlignPosToTabSize(cursor.col) : cursor.col + 1;
			cursor.ch++;
			cursor.offset++;
			ch = originalLine->getCharAtCursor(cursor);
		}
	}
}

const yuki_line_string* yuki_line_string_manager::cloneLineString(const yuki_line_string* str)
{
	return m_allocator->cloneObject(*str);
}

void yuki_line_string_manager::freeLineString(const yuki_line_string* str)
{
	return m_allocator->freeObject(const_cast<yuki_line_string*>(str));
}

yuki_line_string* yuki_line_string_manager::allocLineString(yuki_file_string* fileString)
{
	return m_allocator->allocObject(fileString);
}
