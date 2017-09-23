#pragma once

class YukiCommentNode : public YukiNode
{

};

class YukiComment : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region);
	virtual bool match() { return true; }
};