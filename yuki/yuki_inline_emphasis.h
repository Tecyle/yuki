#pragma once

class YukiInlineEmphasisNode : public YukiNode
{

};

class YukiInlineEmphasis : public YukiInlineFormat
{
public:
	YukiInlineEmphasis();

protected:
	virtual YukiNode* allocNode() override;
};