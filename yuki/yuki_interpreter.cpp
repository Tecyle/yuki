#include "stdafx.h"
#include "yuki_document.h"
#include "yuki_interpreter.h"
#include "yuki_document_node.h"
#include "yuki_region_manager.h"

yuki_document_node* yuki_interpreter::parseYukiToTree()
{
	delete m_documentNode;
	m_documentNode = nullptr;

	yuki_document_node* docNode = new yuki_document_node();

	yuki_document* docParser = dynamic_cast<yuki_document*>(m_session.getParser(yuki_document_name));
	assert(docParser != nullptr);

	if (!docParser->parse(docNode, yukiRegionManager()->getWholeBufferRegion()))
	{
		delete docNode;
		return nullptr;
	}

	m_documentNode = docNode;
	return docNode;
}
