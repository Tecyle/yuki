#pragma once

class YukiInlineUnderlineNode : public YukiNode
{

};

class YukiInlineUnderline : public YukiInlineFormat
{
public:
	YukiInlineUnderline();

protected:
	virtual YukiNode* allocNode() override;
};