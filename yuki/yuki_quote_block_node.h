#pragma once

class yuki_quote_block_node : public yuki_node
{
public:
	__inline void setQuoteType(wstring quoteType) { m_quoteType = quoteType; }
	__inline const wstring& getQuoteType() const { return m_quoteType; }

protected:
	wstring m_quoteType;
};