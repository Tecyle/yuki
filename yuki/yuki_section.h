#pragma once

class YukiSectionNode : public YukiNode 
{
public:
	YukiSectionNode();

private:
	int m_sectionLevel;
};

class YukiSection : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;

private:
	const yuki_region* searchSectionBodyRegion();
};