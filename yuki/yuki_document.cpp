#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_region_ex.h"
#include "yuki.h"
#include "yuki_body.h"
#include "yuki_document.h"

YukiDocumentNode::YukiDocumentNode(const yuki_region* region)
	: YukiNode(nullptr, region)
{
	m_nodeName = L"document";
	m_nodeType = Yuki_blockType;
}

YukiDocument::YukiDocument(YukiGlobal* globalData)
	: YukiStruct(globalData)
{
	m_name = L"document";
	m_type = Yuki_blockType;
}

YukiDocumentNode* YukiDocument::parseYukiDocument()
{
	YukiLinedRegion region;
	YukiDocumentNode* documentNode = new YukiDocumentNode(&region);

	if (parse(documentNode, &region))
		return documentNode;

	delete documentNode;
	return nullptr;
}

// Document : Body
bool YukiDocument::parse(YukiNode* parent, const yuki_region* region)
{
	return getParser(L"body")->parse(parent, region);
}
