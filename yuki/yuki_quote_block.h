#pragma once

enum YukiQuoteLevel
{
	QuoteLevel_default,
	QuoteLevel_danger,
	QuoteLevel_warning,
	QuoteLevel_info,
};

class YukiQuoteBlockAttribute : public YukiStruct
{
public:
	YukiQuoteBlockAttribute(YukiFileLoader* fileLoader, int indentLevel);

	virtual bool parse(YukiStruct* parent);
};

class YukiQuoteBlock : public YukiStruct
{
public:
	YukiQuoteBlock(YukiFileLoader* fileLoader, int indentLevel);

private:
	YukiQuoteLevel m_quoteLevel;
};