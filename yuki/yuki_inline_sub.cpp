#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_format.h"
#include "yuki_inline_sub.h"

YukiInlineSub::YukiInlineSub()
{
	m_prefix = m_suffix = L"vv";
	m_infix = nullptr;
	m_allowEscapeNearMark = false;
}

yuki_node* YukiInlineSub::allocNode()
{
	return new YukiInlineSubNode;
}
