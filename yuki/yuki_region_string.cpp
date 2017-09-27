#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_region_ex.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"
#include "yuki_line_string_manager.h"

static int _countRegionLines(yuki_file_string* fileString, const yuki_region* region)
{
	if (region->end().isValid())
		return region->end().ln - region->begin().ln;
	return fileString->getLineCount() - region->begin().ln;
}

yuki_region_string::yuki_region_string(yuki_file_string* parent, const yuki_region* region)
	: m_parent(parent)
	, m_region(region)
	, m_lineCount(0)
	, m_lines(nullptr)
{
}

void yuki_region_string::buildLineStringArray()
{
	auto mgr = yukiLineStringManager();
	m_lineCount = _countRegionLines(m_parent, m_region);
	m_lines = static_cast<yuki_line_string**>(malloc(sizeof(yuki_line_string*) * m_lineCount));

	for (int i = 0; i < m_lineCount; ++i)
	{
		m_lines[i] = mgr->allocLineStringForRegionString(this, i);
	}
}
