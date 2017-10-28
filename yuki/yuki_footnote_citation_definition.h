#pragma once

class yuki_footnote_citation_definition : public yuki_structure_parser
{
public:
	yuki_footnote_citation_definition(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_footnote_citation_definition() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }

protected:
	bool parseLabel(wstring& label, bool& isFootnote);
};