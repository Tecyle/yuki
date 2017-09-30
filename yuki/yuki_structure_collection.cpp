#include "stdafx.h"
#include "yuki_structure.h"
#include "yuki_structure_collection.h"
#include "yuki_string_hashmap.h"
#include "yuki_session.h"

typedef yuki_string_hashmap<yuki_structure*> yuki_structure_map;

class yuki_structure_collection_imple
{
public:
	yuki_structure_collection_imple(yuki_session* session);
	~yuki_structure_collection_imple();

public:
	__inline yuki_structure* getStructure(const wchar_t* name);

private:
	yuki_structure_map m_structureMap;
};

yuki_structure_collection_imple::yuki_structure_collection_imple(yuki_session* session)
{
#define yuki_add_structure(name, instance) \
	m_structureMap.insert(make_pair(L##name, new instance(session)))

	yuki_add_structure("document", yuki_document);

#undef yuki_add_structure
}
