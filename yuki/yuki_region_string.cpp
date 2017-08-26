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
	m_lines = parent->getLine(region->getSLn());
}

YukiLinedRegionString::~YukiLinedRegionString()
{ 
	delete m_region; 
}