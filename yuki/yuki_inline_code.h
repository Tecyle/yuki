#pragma once

class YukiInlineCodeNode : public YukiNode
{

};

class YukiInlineCode : public YukiInlineFormat
{
public:
	YukiInlineCode();

protected:
	virtual YukiNode* allocNode() override;
};