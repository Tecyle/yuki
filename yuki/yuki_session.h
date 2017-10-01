#pragma once

class yuki_session_variables;

class yuki_structure_parser_collection;
class yuki_file_reader;
class yuki_document_node;

class yuki_session
{
public:
	yuki_session();
	virtual ~yuki_session();

public:
	__inline yuki_structure_parser_collection* getStructureParserCollection();
	__inline yuki_file_reader* getFileReader();
	__inline yuki_document_node* getRootNode();

private:
	yuki_session_variables* m_variables;
};