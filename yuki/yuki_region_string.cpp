#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_region_ex.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"

YukiLinedRegionString::YukiLinedRegionString(YukiFileString* parent, const YukiRegion* region)
	: YukiRegionString(parent)
{
	m_region = new YukiLinedRegion(dynamic_cast<const YukiLinedRegion*>(region));

	assert(m_region->getIndent() != YUKI_ERROR_INDENT);
	assert(m_region->getRegionType() == Yuki_linedRegion);

	m_lineCount = region->getELn() == -1 ? parent->getLineCount() : region->getELn();
	m_lineCount -= region->getSLn();
	
	m_lines = (YukiLineString*)malloc(m_lineCount * sizeof(YukiLineString));
	if (m_lineCount == 1)
	{
		m_lines[0].YukiLineString::YukiLineString(parent->getLine(region->getSLn()), region->getSCol(), region->getECol());
		return;
	}

	m_lines[0].YukiLineString::YukiLineString(parent->getLine(region->getSLn()), region->getSCol(), -1);
	for (int i = 1; i < m_lineCount - 1; ++i)
	{
		m_lines[i].YukiLineString::YukiLineString(parent->getLine(region->getSLn() + i));
	}
	m_lines[m_lineCount - 1].YukiLineString::YukiLineString(parent->getLine(region->getSLn()), 0, region->getECol());
}

YukiLinedRegionString::~YukiLinedRegionString()
{ 
	delete m_region;
}