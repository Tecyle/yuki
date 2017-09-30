#pragma once

class YukiInlineEmphasisNode : public yuki_node
{

};

class YukiInlineEmphasis : public YukiInlineFormat
{
public:
	YukiInlineEmphasis();

protected:
	virtual yuki_node* allocNode() override;
};