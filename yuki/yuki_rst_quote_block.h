#pragma once

class YukiRstQuoteBlock : public YukiQuoteBlock
{
public:
	YukiRstQuoteBlock(YukiFileLoader* fileLoader, int indentLevel);

	virtual void parse(YukiStruct* parent);

protected:
	void searchingBlockRegion();

private:
	int m_quoteBlockIndent;
	YukiBlockRegion m_bodyRegion;
	YukiBlockRegion m_attrRegion;
};