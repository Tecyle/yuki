#pragma once

class YukiSectionNode : public yuki_node 
{
public:
	YukiSectionNode();

private:
	int m_sectionLevel;
};

class YukiSection : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;

private:
	const yuki_region* searchSectionBodyRegion();
};