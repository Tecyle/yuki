#pragma once

class YukiInlineStrongNode : public YukiNode
{

};

class YukiInlineStrong : public YukiInlineFormat
{
public:
	YukiInlineStrong();

protected:
	virtual YukiNode* allocNode() override;
};