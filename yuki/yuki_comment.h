#pragma once

class YukiCommentNode : public YukiNode
{

};

class YukiComment : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region);
	virtual bool match() { return true; }
};