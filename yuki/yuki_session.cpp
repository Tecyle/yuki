#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_session.h"
#include "yuki_settings.h"
#include "yuki_file_reader.h"

class yuki_session_variables
{
public:
	yuki_session_variables();
	~yuki_session_variables();

	yuki_settings m_yukiSettings;			///< 当前会话中的所有的配置选项
	yuki_file_reader m_fileReader;			///< 当前会话中的文件加载器
};

yuki_session::yuki_session()
	: m_matcherCollection(this)
{
#define FOLLOW_SET(x, y) m_matcherCollection.addFollowSet(L##x, L##y)
	// body 的后继节点
	FOLLOW_SET("body", "rst_quote_block");
#undef FOLLOW_SET
}
