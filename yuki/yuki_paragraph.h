#pragma once

class YukiParagraphNode : public YukiNode
{

};

class YukiParagraph : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override { return true; }
};