#pragma once

class YukiSectionNode : public YukiNode 
{
public:

private:
	int m_sectionLevel;
};

class YukiSection : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;

private:
};