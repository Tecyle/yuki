#pragma once

class YukiInlineSupNode : public YukiNode
{

};

class YukiInlineSup : public YukiInlineFormat
{
public:
	YukiInlineSup();

protected:
	virtual YukiNode* allocNode() override;
};