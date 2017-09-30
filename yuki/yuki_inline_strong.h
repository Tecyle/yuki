#pragma once

class YukiInlineStrongNode : public yuki_node
{

};

class YukiInlineStrong : public YukiInlineFormat
{
public:
	YukiInlineStrong();

protected:
	virtual yuki_node* allocNode() override;
};