#pragma once

class yuki_structure_parser;
class yuki_structure_parser_collection_imple;

class yuki_structure_parser_collection
{
public:
	yuki_structure_parser_collection(yuki_session* session);
	~yuki_structure_parser_collection();

public:
	yuki_structure_parser* getStructureParserByName(const wchar_t* name);
	vector<yuki_structure_parser*>* getFollowParsersByName(const wchar_t* name);

private:
	yuki_structure_parser_collection_imple* m_collections;
	yuki_session* m_session;
};