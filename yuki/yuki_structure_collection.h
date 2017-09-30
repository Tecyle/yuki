#pragma once

class yuki_structure;
class yuki_structure_collection_imple;

class yuki_structure_collection
{
public:
	yuki_structure_collection(yuki_session* session);
	~yuki_structure_collection();

public:
	yuki_structure* getStructureByName(const wchar_t* name);

private:
	yuki_structure_collection_imple* m_collections;
	yuki_session* m_session;
};