#pragma once

class yuki_document : public yuki_structure_parser
{
public:
	yuki_document(yuki_session* globalData);
	virtual ~yuki_document() {}

public:
	virtual bool parse(yuki_node* parent, const yuki_region* region) override;
	virtual bool match() { return true; }
};