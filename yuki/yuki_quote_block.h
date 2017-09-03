#pragma once

enum yuki_quote_level
{
	QuoteLevel_default,
	QuoteLevel_danger,
	QuoteLevel_warning,
	QuoteLevel_info,
};

class YukiQuoteBlockAttributeNode : public YukiNode
{

};

class YukiQuoteBlockAttribute : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;
	virtual bool matchNoBackward();
};

class YukiQuoteBlockNode : public YukiNode
{
public:
	void setQuoteLevel(yuki_quote_level level);

private:
	yuki_quote_level m_quoteLevel;
};
