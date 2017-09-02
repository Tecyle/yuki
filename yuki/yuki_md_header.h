#pragma once

class YukiMdHeaderNode : public YukiNode
{
public:
	__inline void setHeaderLevel(int headerLevel) { m_headerLevel = headerLevel; }

private:
	int m_headerLevel;
};

class YukiMdHeader : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;
};