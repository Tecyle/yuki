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

	yuki_settings m_yukiSettings;			///< ��ǰ�Ự�е����е�����ѡ��
	yuki_file_reader m_fileReader;			///< ��ǰ�Ự�е��ļ�������
};

yuki_session::yuki_session()
	: m_matcherCollection(this)
{
#define FOLLOW_SET(x, y) m_matcherCollection.addFollowSet(L##x, L##y)
	// body �ĺ�̽ڵ�
	FOLLOW_SET("body", "rst_quote_block");
#undef FOLLOW_SET
}
