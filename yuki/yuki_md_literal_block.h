#pragma once

class yuki_md_literal_block : public yuki_structure_parser
{
public:
	yuki_md_literal_block(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_md_literal_block() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();

private:
	wstring m_language;
};