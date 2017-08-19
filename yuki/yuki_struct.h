#pragma once

class YukiStruct
{
public:
	virtual YukiStruct* getParent() const = 0;
	virtual YukiStruct* getRoot() const = 0;

	virtual YukiNode* getNode() const = 0;

	virtual void appendChild(YukiStruct* child) = 0;
	virtual void appendChildByRegion(YukiStruct* child, const YukiBlockRegion* region) = 0;

	virtual void parse(YukiStruct* parent) = 0;

	virtual bool outOfRegion() const { return m_limitRegion == nullptr || m_fileLoader->getLineNum() >= m_limitRegion->endLineNum; }

	YukiStruct(YukiFileLoader* fileLoader, int indentLevel)
		: m_fileLoader(fileLoader)
		, m_indentLevel(indentLevel)
	{}

protected:
	const YukiBlockRegion* m_limitRegion;
	YukiStruct* m_parent;
	YukiFileLoader* m_fileLoader;
	int m_indentLevel;
};