#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_node.h"
#include "yuki_structure_parser.h"
#include "yuki_session.h"
#include "yuki_structure_parser_collection.h"

yuki_structure_parser::yuki_structure_parser(yuki_session * session)
	: m_session(session)
	, m_type(yuki_blockStructure)
	, m_name(nullptr)
{
}

yuki_structure_parser::~yuki_structure_parser()
{
}

yuki_structure_parser* yuki_structure_parser::getParser(const wchar_t* name)
{
	return m_session->getStructureParserCollection()->getStructureParserByName(name);
}

yuki_file_reader* yuki_structure_parser::getFileReader()
{
	return m_session->getFileReader();
}
