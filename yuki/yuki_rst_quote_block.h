#pragma once
#include "yuki_quote_block.h"

class YukiRstQuoteBlock : public YukiQuoteBlock
{
public:
	YukiRstQuoteBlock(YukiFileLoader* fileLoader, int indentLevel);

	virtual bool parse(YukiStruct* parent);

protected:
	void searchingBlockRegion();

private:
	int m_quoteBlockIndent;
	YukiBlockRegion m_bodyRegion;
	YukiBlockRegion m_attrRegion;
};