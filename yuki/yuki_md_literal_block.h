#pragma once

class YukiLiteralBlockNode : public yuki_node
{
public:
	void setLanguage(wstring language);
	const wstring& getLanguage();
};

class YukiMdLiteralBlock : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();

private:
	wstring m_language;
};