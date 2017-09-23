#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"

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

	free(buffer);
	return true;
}

const YukiLineString* yuki_file_string::getLine(int index)
{
	if (m_lineCount == 0)
		return nullptr;

	index = yuki_get_absolute_index(index, m_lineCount);
	if (index >= 0 && index < m_lineCount)
		return m_lines + index;

	return nullptr;
}

const YukiRegionString* yuki_file_string::getRegionString(const yuki_region* region)
{
	switch (region->getRegionType())
	{
	case Yuki_linedRegion:
		return new YukiLinedRegionString(this, region);
	case Yuki_blockRegion:
		return new YukiBlockRegionString(this, region);
	default:
		assert(!"Not supported type.");
	}

	return nullptr;
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

	const YukiLineString* line = getLine(target.ln);
	assert(line);
	target.ch = line->getChByCol(target.col);
	target.offset = line->getOffset() + target.ch;
}

bool yuki_file_string::buildFromBuffer()
{
	assert(m_buffer != nullptr);

	free(m_lines);

	m_lineCount = countLines(m_buffer);
	m_lines = (YukiLineString*)malloc(m_lineCount * sizeof(YukiLineString));
	
	wchar_t* p = m_buffer;
	for (int i = 0; i < m_lineCount; ++i)
	{
		YukiLineString* line = m_lines + i;
		new(line) YukiLineString(this, i, p);
	}

	assert(*p == 0);
	return true;
}
