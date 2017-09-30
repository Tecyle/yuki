#pragma once

class YukiPlainTextNode : public yuki_node
{
public:
	__inline wstring& plainText() { return m_text; }
private:
	wstring m_text;
};

class YukiPlainText : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};
