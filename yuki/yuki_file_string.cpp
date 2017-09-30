#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"
#include "yuki_line_string_manager.h"
#include "yuki_region_manager.h"

static int countLines(const wchar_t* str)
{
	int res = 1;

	while (*str)
	{
		if (*str == '\n')
			res++;
	}

	return res;
}

yuki_file_string::yuki_file_string()
{
	m_buffer = nullptr;
	m_lines = nullptr;
	m_bufferLength = 0;
	m_lineCount = 0;
}

yuki_file_string::~yuki_file_string()
{
	free(m_buffer);
	free(m_lines);
}

// 一个 yuki 文件不会太大，所以一次性载入内存
bool yuki_file_string::loadFromFile(const wchar_t* fileName)
{
	FILE* fp = nullptr;
	_wfopen_s(&fp, fileName, L"rb");
	if (fp == nullptr)
		return false;

	fseek(fp, SEEK_END, 0);
	size_t fileLength = ftell(fp) + 1;
	fseek(fp, SEEK_SET, 0);
	char* buffer = (char*)malloc(fileLength);
	size_t readLength = fread_s(buffer, fileLength, 1, fileLength - 1, fp);
	assert(readLength == fileLength - 1);
	fclose(fp);
	
	// 编码转换
	free(m_buffer);
	m_bufferLength = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, nullptr, 0);
	m_buffer = (wchar_t*)malloc(m_bufferLength * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, buffer, -1, m_buffer, m_bufferLength);

	// 构建所有行信息
	buildFromBuffer();
	buildWholeFileRegionString();

	free(buffer);
	return true;
}

const yuki_line_string* yuki_file_string::getLine(int index)
{
	if (m_lineCount == 0)
		return nullptr;

	index = yuki_get_absolute_index(index, m_lineCount);
	if (index >= 0 && index < m_lineCount)
		return *m_lines + index;

	return nullptr;
}

const yuki_region_string* yuki_file_string::allocRegionString(const yuki_region* region)
{
	if (region == nullptr)
		return new yuki_region_string(m_wholeFileRegionString);

	return new yuki_region_string(this, region);
}

/*
	坐标相加方法：
	1. 行号直接相加；
	2. 列号直接相加；
	3. 定位到相加后的行；
	4. 计数行内字符数；
	5. 总偏移为行偏移加上行内字符数。
*/
void yuki_file_string::addCursorOffsetToCursor(yuki_cursor& target, const yuki_cursor& offset)
{
	if (!target.isValid())
		return;

	assert(offset.isValid());

	target.ln += offset.ln;
	target.col += offset.col;

	const yuki_line_string* line = getLine(target.ln);
	assert(line);
	target.ch = line->getChByCol(target.col);
	target.offset = line->getOffset() + target.ch;
}

bool yuki_file_string::buildFromBuffer()
{
	assert(m_buffer != nullptr);

	free(m_lines);

	m_lineCount = countLines(m_buffer);
	m_lines = (const yuki_line_string**)malloc(m_lineCount * sizeof(const yuki_line_string*));
	
	wchar_t* p = m_buffer;
	for (int i = 0; i < m_lineCount; ++i)
	{
		const yuki_line_string** line = m_lines + i;
		*line = yukiLineStringManager()->allocLineStringForFileString(this, i, p);
	}

	assert(*p == 0);
	return true;
}

void yuki_file_string::buildWholeFileRegionString()
{
	delete m_wholeFileRegionString;
	const yuki_region* region = yukiRegionManager()->allocRegion(yuki_linedRegion,
		yuki_cursor(0, 0, 0, 0), yuki_cursor(), 0);
	m_wholeFileRegionString = new yuki_region_string(this, region, m_lines, m_lineCount);
}
