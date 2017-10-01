#include "stdafx.h"
#include "yuki_structure_parser.h"
#include "yuki_structure_parser_collection.h"
#include "yuki_string_hashmap.h"
#include "yuki_session.h"

typedef yuki_string_hashmap<yuki_structure_parser*> yuki_structure_map;

class yuki_structure_parser_collection_imple
{
public:
	yuki_structure_parser_collection_imple(yuki_session* session);
	~yuki_structure_parser_collection_imple();

public:
	__inline yuki_structure_parser* getStructure(const wchar_t* name);

private:
	yuki_structure_map m_structureMap;
};

yuki_structure_parser_collection_imple::yuki_structure_parser_collection_imple(yuki_session* session)
{
#define yuki_add_structure(name, instance) \
	m_structureMap.insert(make_pair(L##name, new instance(session)))

	yuki_add_structure("document", yuki_document);

#undef yuki_add_structure
}
