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

YukiFileString::YukiFileString()
{
	m_buffer = nullptr;
	m_lines = nullptr;
	m_bufferLength = 0;
	m_lineCount = 0;
}

YukiFileString::~YukiFileString()
{
	free(m_buffer);
	free(m_lines);
}

const YukiLineString* YukiFileString::getLine(int index)
{
	if (m_lineCount == 0)
		return nullptr;

	index = yuki_get_absolute_index(index, m_lineCount);
	if (index >= 0 && index < m_lineCount)
		return m_lines + index;

	return nullptr;
}

const YukiRegionString* YukiFileString::getRegionString(const YukiRegion* region)
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

bool YukiFileString::buildFromBuffer()
{
	assert(m_buffer != nullptr);

	free(m_lines);

	m_lineCount = countLines(m_buffer);
	m_lines = (YukiLineString*)malloc(m_lineCount * sizeof(YukiLineString));
	
	wchar_t* p = m_buffer;
	for (int i = 0; i < m_lineCount; ++i)
	{
		YukiLineString* line = m_lines + i;
		line->YukiLineString::YukiLineString(this, i, p);
	}

	assert(*p == 0);
	return true;
}
