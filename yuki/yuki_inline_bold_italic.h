#pragma once

class YukiInlineBoldItalicNode : public YukiNode
{

};

class YukiInlineBoldItalic : public YukiInlineFormat
{
public:
	YukiInlineBoldItalic();

protected:
	virtual YukiNode* allocNode() override;
};