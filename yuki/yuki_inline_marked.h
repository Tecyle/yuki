#pragma once

class YukiInlineMarkedNode : public yuki_node
{

};

class YukiInlineMarked : public YukiInlineFormat
{
public:
	YukiInlineMarked();

protected:
	virtual yuki_node* allocNode() override;
};