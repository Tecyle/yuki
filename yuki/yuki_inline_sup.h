#pragma once

class YukiInlineSupNode : public yuki_node
{

};

class YukiInlineSup : public YukiInlineFormat
{
public:
	YukiInlineSup();

protected:
	virtual yuki_node* allocNode() override;
};