#pragma once

class YukiInlineKeyNode : public YukiNode
{

};

class YukiInlineKey : public YukiInlineFormat
{
public:
	YukiInlineKey();

protected:
	virtual YukiNode* allocNode() override;
};