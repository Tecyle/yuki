#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_body.h"
#include "yuki_document.h"

YukiDocumentNode::YukiDocumentNode(const yuki_region* region)
	: yuki_node(nullptr, region)
{
	m_nodeName = L"document";
	m_nodeType = Yuki_blockType;
}

yuki_document::yuki_document(yuki_session* globalData)
	: yuki_structure(globalData)
{
	m_name = L"document";
	m_type = Yuki_blockType;
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
