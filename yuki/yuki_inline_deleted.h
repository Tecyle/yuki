#pragma once

class YukiInlineDeletedNode : public yuki_node
{

};

class YukiInlineDeleted : public YukiInlineFormat
{
public:
	YukiInlineDeleted();

protected:
	virtual yuki_node* allocNode() override;
};