#pragma once

class YukiBodyNode : public YukiNode
{

};

class YukiBody : public YukiStruct
{
public:
	YukiBody(YukiGlobal* globalData);

public:
	virtual bool parse(YukiNode* parent, const YukiRegion* region) override;
};