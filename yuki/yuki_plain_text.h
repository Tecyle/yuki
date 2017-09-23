#pragma once

class YukiPlainTextNode : public YukiNode
{
public:
	__inline wstring& plainText() { return m_text; }
private:
	wstring m_text;
};

class YukiPlainText : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};
