#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_matcher.h"
#include "yuki_global.h"

YukiGlobal::YukiGlobal()
	: m_matcherCollection(this)
{
#define FOLLOW_SET(x, y) m_matcherCollection.addFollowSet(L##x, L##y)
	// body 的后继节点
	FOLLOW_SET("body", "rst_quote_block");
#undef FOLLOW_SET
}
