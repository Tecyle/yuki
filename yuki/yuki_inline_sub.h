#pragma once

class YukiInlineSubNode : public YukiNode
{

};

class YukiInlineSub : public YukiInlineFormat
{
public:
	YukiInlineSub();

protected:
	virtual YukiNode* allocNode() override;
};