#pragma once

class YukiParagraphNode : public yuki_node
{

};

class YukiParagraph : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};