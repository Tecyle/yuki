#pragma once

class YukiDocumentNode : public YukiNode
{
public:
};

class YukiDocument : public YukiStruct
{
public:
	inline virtual YukiNode* getNode() const { return m_node; }

	virtual bool parse(YukiStruct* parent);

public:
	YukiDocument(YukiFileLoader* m_fileLoader, int indentLevel);

private:
	YukiDocumentNode* m_node;
};