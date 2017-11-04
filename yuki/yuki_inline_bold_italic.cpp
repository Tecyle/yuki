#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_format.h"
#include "yuki_inline_bold_italic.h"
#include "yuki_inline_bold_italic_node.h"

yuki_inline_bold_italic::yuki_inline_bold_italic(yuki_session* globalData)
	: yuki_inline_format(globalData)
{
	m_prefix = m_suffix = L"***";
	m_infix = nullptr;
	m_allowEscapeNearMark = false;
}

yuki_node* yuki_inline_bold_italic::allocNode()
{
	return new yuki_inline_bold_italic_node;
}
