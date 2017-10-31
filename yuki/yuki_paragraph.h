#pragma once

class yuki_paragraph : public yuki_structure_parser
{
public:
	yuki_paragraph(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_paragraph() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};