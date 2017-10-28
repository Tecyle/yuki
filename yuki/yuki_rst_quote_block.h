#pragma once
#include "yuki_quote_block.h"

class yuki_rst_quote_block : public yuki_structure_parser
{
public:
	yuki_rst_quote_block(yuki_session* globalData);
	virtual ~yuki_rst_quote_block() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	void searchingBlockRegion(const yuki_region* &bodyRegion, const yuki_region* &attrRegion);

private:
	int m_quoteBlockIndent;
};