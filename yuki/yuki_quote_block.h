#pragma once

class YukiQuoteBlockAttributeNode : public YukiNode
{

};

class YukiQuoteBlockAttribute : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override;
	virtual bool matchNoBackward();
};

class YukiQuoteBlockNode : public YukiNode
{
public:
	void setQuoteCategory(wstring categoryName);

private:
};
