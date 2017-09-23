#pragma once
#include "yuki_quote_block.h"

class YukiRstQuoteBlock : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	void searchingBlockRegion(const yuki_region* &bodyRegion, const yuki_region* &attrRegion);

private:
	int m_quoteBlockIndent;
};