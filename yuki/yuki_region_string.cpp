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
	int result = 0;
	if (region->end().isValid())
	{
		result = region->end().ln - region->begin().ln;
		if (region->end().col == 0)
			result--;
	}
	else
	{
		result = fileString->getLineCount() - region->begin().ln;
	}

	assert(result > 0);
	return result;
}

yuki_region_string::yuki_region_string(yuki_file_string* parent, const yuki_region* region)
	: m_parent(parent)
	, m_region(region)
	, m_lineCount(0)
	, m_lines(nullptr)
	, m_needRelease(true)
{
	buildLineStringArray();
}

yuki_region_string::yuki_region_string(const yuki_region_string* r)
	: m_parent(r->m_parent)
	, m_region(r->m_region)
	, m_lineCount(r->m_lineCount)
	, m_lines(r->m_lines)
	, m_needRelease(false)
{
}

yuki_region_string::~yuki_region_string()
{
	if (m_needRelease)
	{
		free(m_lines);
	}
}

const yuki_line_string* yuki_region_string::getLineStringAtCursor(const yuki_cursor& cursor) const
{
	assert(cursor.isValid());
	int ln = cursor.ln - m_region->begin().ln;
	if (ln >= 0 && ln < m_lineCount)
		return m_lines[ln];
	return nullptr;
}

void yuki_region_string::buildLineStringArray()
{
	auto mgr = yukiLineStringManager();
	m_lineCount = _countRegionLines(m_parent, m_region);
	m_lines = static_cast<yuki_line_string**>(malloc(sizeof(yuki_line_string*) * m_lineCount));

	for (int i = 0; i < m_lineCount; ++i)
	{
		m_lines[i] = mgr->allocLineStringForRegionString(m_parent, m_region, i);
		assert(m_lines[i]);
	}
}
