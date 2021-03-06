#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_format.h"
#include "yuki_inline_code.h"

YukiInlineCode::YukiInlineCode()
{
	m_prefix = m_suffix = L"``";
	m_infix = nullptr;
	m_allowEscapeNearMark = true;
}

yuki_node* YukiInlineCode::allocNode()
{
	return new YukiInlineCodeNode;
}
