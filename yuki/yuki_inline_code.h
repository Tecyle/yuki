#pragma once

class YukiInlineCodeNode : public yuki_node
{

};

class YukiInlineCode : public YukiInlineFormat
{
public:
	YukiInlineCode();

protected:
	virtual yuki_node* allocNode() override;
};