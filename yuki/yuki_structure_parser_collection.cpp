#include "stdafx.h"
#include "yuki_structure_parser.h"
#include "yuki_structure_parser_collection.h"
#include "yuki_string_hashmap.h"
#include "yuki_session.h"
// parsers
#include "yuki_document.h"
#include "yuki_body.h"
#include "yuki_quote_block.h"
#include "yuki_rst_quote_block.h"
#include "yuki_explicit_markup.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_hyperlink_target.h"
#include "yuki_substitution_definition.h"
#include "yuki_directives.h"

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
	yuki_add_structure(yuki_rst_quote_block_name, yuki_rst_quote_block);
	yuki_add_structure(yuki_explicit_markup_block_name, yuki_explicit_markup);
	yuki_add_structure(yuki_footnote_citation_definition_block_name, yuki_footnote_citation_definition);
	yuki_add_structure(yuki_hyperlink_target_block_name, yuki_hyperlink_target);
	yuki_add_structure(yuki_substitution_definition_block_name, yuki_substitution_definition);
	yuki_add_structure(yuki_directives_block_name, yuki_directives);

#undef yuki_add_structure

#define yuki_add_follow_set(parent, child) \
	m_structureFollowSet[parent].push_back(getStructure(child))

	yuki_add_follow_set(yuki_body_name, yuki_rst_quote_block_name);
	yuki_add_follow_set(yuki_body_name, yuki_explicit_markup_block_name);
	// TODO 将显示标记块改写成后跟集合模式
	yuki_add_follow_set(yuki_explicit_markup_block_name, yuki_footnote_citation_definition_block_name);
	yuki_add_follow_set(yuki_explicit_markup_block_name, yuki_hyperlink_target_block_name);
	yuki_add_follow_set(yuki_explicit_markup_block_name, yuki_substitution_definition_block_name);
	yuki_add_follow_set(yuki_explicit_markup_block_name, yuki_directives_block_name);

#undef yuki_add_follow_set
	
}
