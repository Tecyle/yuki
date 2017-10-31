#pragma once

class yuki_comment : public yuki_structure_parser
{
public:
	yuki_comment(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_comment() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region);
	virtual bool match() { return true; }
};