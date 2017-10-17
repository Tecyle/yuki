#include "stdafx.h"
#include "yuki_structure_parser.h"
#include "yuki_structure_parser_collection.h"
#include "yuki_string_hashmap.h"
#include "yuki_session.h"
#include "yuki_document.h"
#include "yuki_body.h"
#include "yuki_quote_block.h"

typedef yuki_string_hashmap<yuki_structure_parser*> yuki_structure_map;
typedef yuki_string_hashmap<vector<yuki_structure_parser*>> yuki_structure_follow_set;

class yuki_structure_parser_collection_imple
{
public:
	yuki_structure_parser_collection_imple(yuki_session* session);
	~yuki_structure_parser_collection_imple();

public:
	__inline yuki_structure_parser* getStructure(const wchar_t* name);
	__inline vector<yuki_structure_parser*>* getFollowParserByName(const wchar_t* name);

private:
	yuki_structure_map m_structureMap;
	yuki_structure_follow_set m_structureFollowSet;
};

yuki_structure_parser_collection_imple::yuki_structure_parser_collection_imple(yuki_session* session)
{
#define yuki_add_structure(name, instance) \
	m_structureMap.insert(make_pair(name, new instance(session)))

	yuki_add_structure(yuki_document_name, yuki_document);
	yuki_add_structure(yuki_body_name, yuki_body);
	yuki_add_structure(yuki_quote_block_attribute_name, yuki_quote_block_attribute);

#undef yuki_add_structure

#define yuki_add_follow_set(parent, child) \
	m_structureFollowSet[parent].push_back(getStructure(child))

	yuki_add_follow_set(yuki_body_name, yuki_quote_block_name);

#undef yuki_add_follow_set
	
}
