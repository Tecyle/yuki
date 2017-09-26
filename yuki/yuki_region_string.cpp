#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_region_ex.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"
#include "yuki_line_string_manager.h"

yuki_region_string::yuki_region_string(yuki_file_string* parent, const yuki_region* region)
	: m_parent(parent)
	, m_region(region)
	, m_lineCount(0)
	, m_lines(nullptr)
{
}
