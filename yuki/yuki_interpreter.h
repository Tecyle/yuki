#pragma once

class yuki_interpreter
{
public:

protected:
	yuki_document_node* parseYukiToTree();

private:
	yuki_session m_session;
	yuki_document_node* m_documentNode;
};