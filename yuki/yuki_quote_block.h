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
	virtual void parse(YukiStruct* parent);
};

class YukiQuoteBlock : public YukiStruct
{
private:
	YukiQuoteLevel m_quoteLevel;
};