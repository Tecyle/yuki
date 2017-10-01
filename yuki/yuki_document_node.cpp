#include "stdafx.h"
#include "yuki_node.h"
#include "yuki_document_node.h"

yuki_document_node::yuki_document_node()
{
	m_nodeName = _T(yuki_document_name);
	m_nodeType = yuki_blockStructure;
}

yuki_document_node::~yuki_document_node()
{
}
