#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_structure_parser.h"
#include "yuki_body.h"
#include "yuki_document.h"

yuki_document::yuki_document(yuki_session* globalData)
	: yuki_structure_parser(globalData)
{
	m_name = _T(yuki_document_name);
	m_type = yuki_blockStructure;
}

YukiDocumentNode* yuki_document::parseYukiDocument()
{
	YukiLinedRegion region;
	YukiDocumentNode* documentNode = new YukiDocumentNode(&region);

	if (parse(documentNode, &region))
		return documentNode;

	delete documentNode;
	return nullptr;
}

// Document : Body
bool yuki_document::parse(yuki_node* parent, const yuki_region* region)
{
	return getParser(L"body")->parse(parent, region);
}
