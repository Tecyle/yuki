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

	yuki_settings m_yukiSettings;			///< ��ǰ�Ự�е����е�����ѡ��
	yuki_file_reader m_fileReader;			///< ��ǰ�Ự�е��ļ�������
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
