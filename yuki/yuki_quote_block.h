#pragma once

class YukiQuoteBlockAttributeNode : public yuki_node
{

};

class YukiQuoteBlockAttribute : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;
	virtual bool matchNoBackward();
};

class YukiQuoteBlockNode : public yuki_node
{
public:
	void setQuoteCategory(wstring categoryName);

private:
};
