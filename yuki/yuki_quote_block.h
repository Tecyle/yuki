#pragma once

class yuki_quote_block_attribute : public yuki_structure_parser
{
public:
	yuki_quote_block_attribute(yuki_session* session);
	virtual ~yuki_quote_block_attribute() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();
};
