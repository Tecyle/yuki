#pragma once
#include "yuki_quote_block.h"

class YukiRstQuoteBlock : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

protected:
	void searchingBlockRegion(const YukiRegion* &bodyRegion, const YukiRegion* &attrRegion);

private:
	int m_quoteBlockIndent;
};