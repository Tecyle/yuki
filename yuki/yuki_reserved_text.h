#pragma once

class YukiReservedTextNode : public YukiNode
{
public:
	__inline wstring& reservedText() { return m_reservedText; }

private:
	wstring m_reservedText;
};

class YukiReservedText : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};