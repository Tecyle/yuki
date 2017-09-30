#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_format.h"
#include "yuki_inline_key.h"

YukiInlineKey::YukiInlineKey()
{
	m_prefix = m_suffix = L"```";
	m_infix = nullptr;
	m_allowEscapeNearMark = true;
}

yuki_node* YukiInlineKey::allocNode()
{
	return new YukiInlineKeyNode;
}
