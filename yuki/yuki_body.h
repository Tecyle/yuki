#pragma once

class yuki_body : public yuki_structure_parser
{
public:
	yuki_body(yuki_session* globalData);
	virtual ~yuki_body() {}

public:
	virtual bool parse(yuki_node* parent, const yuki_region* region) override;
	virtual bool match() override {}
};