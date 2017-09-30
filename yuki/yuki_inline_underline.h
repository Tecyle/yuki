#pragma once

class YukiInlineUnderlineNode : public yuki_node
{

};

class YukiInlineUnderline : public YukiInlineFormat
{
public:
	YukiInlineUnderline();

protected:
	virtual yuki_node* allocNode() override;
};