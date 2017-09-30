#pragma once

class YukiInlineSubNode : public yuki_node
{

};

class YukiInlineSub : public YukiInlineFormat
{
public:
	YukiInlineSub();

protected:
	virtual yuki_node* allocNode() override;
};