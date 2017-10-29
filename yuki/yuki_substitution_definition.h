#pragma once

class yuki_substitution_definition : public yuki_structure_parser
{
public:
	yuki_substitution_definition(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_substitution_definition() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};