#pragma once

class YukiInlineDeletedNode : public YukiNode
{

};

class YukiInlineDeleted : public YukiInlineFormat
{
public:
	YukiInlineDeleted();

protected:
	virtual YukiNode* allocNode() override;
};