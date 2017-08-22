#pragma once
#include "yuki_file_loader.h"

class YukiStruct
{
public:
	virtual YukiStruct* getParent() { return m_parent; }
	virtual bool appendChild(YukiStruct* child);

	virtual bool parse(YukiStruct* parent) = 0;

	virtual bool outOfRegion(int lineNum, int colNum) { return m_limitRegion != nullptr && !m_limitRegion->inRegion(lineNum, colNum); }

	YukiStruct(YukiFileLoader* fileLoader, const YukiBlockRegion* region)
		: m_fileLoader(fileLoader)
		, m_limitRegion(region)
	{}

protected:
	const YukiBlockRegion* m_limitRegion;
	YukiStruct* m_parent;
	YukiFileLoader* m_fileLoader;
	vector<YukiStruct*> m_children;
};

bool YukiStruct::appendChild(YukiStruct* child)
{
	if (!child->parse(this))
		return false;

	m_children.push_back(child);
	return true;
}
