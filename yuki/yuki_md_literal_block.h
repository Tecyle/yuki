#pragma once

class YukiLiteralBlockNode : public YukiNode
{
public:
	void setLanguage(wstring language);
	const wstring& getLanguage();
};

class YukiMdLiteralBlock : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();

private:
	wstring m_language;
};