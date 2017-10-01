#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_session.h"
#include "yuki_settings.h"
#include "yuki_file_reader.h"
#include "yuki_structure_parser_collection.h"

class yuki_session_variables
{
public:
	yuki_session_variables();
	~yuki_session_variables();

	yuki_settings m_yukiSettings;			///< 当前会话中的所有的配置选项
	yuki_file_reader m_fileReader;			///< 当前会话中的文件加载器
	yuki_structure_parser_collection m_structureParserCollection;
};

yuki_session::yuki_session()
	: m_variables(new yuki_session_variables())
{
}

yuki_session::~yuki_session()
{
	delete m_variables;
}

yuki_structure_parser_collection* yuki_session::getStructureParserCollection()
{
	return &m_variables->m_structureParserCollection;
}

yuki_file_reader * yuki_session::getFileReader()
{
	return &m_variables->m_fileReader;
}
