#pragma once

class YukiInlineBoldItalicNode : public yuki_node
{

};

class YukiInlineBoldItalic : public YukiInlineFormat
{
public:
	YukiInlineBoldItalic();

protected:
	virtual yuki_node* allocNode() override;
};