#pragma once

class YukiInlineKeyNode : public yuki_node
{

};

class YukiInlineKey : public YukiInlineFormat
{
public:
	YukiInlineKey();

protected:
	virtual yuki_node* allocNode() override;
};