#pragma once

class YukiInlineMarkedNode : public YukiNode
{

};

class YukiInlineMarked : public YukiInlineFormat
{
public:
	YukiInlineMarked();

protected:
	virtual YukiNode* allocNode() override;
};