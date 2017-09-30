#pragma once

class YukiReservedTextNode : public yuki_node
{
public:
	__inline wstring& reservedText() { return m_reservedText; }

private:
	wstring m_reservedText;
};

class YukiReservedText : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};