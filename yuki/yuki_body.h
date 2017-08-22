#pragma once

class YukiBodyNode : public YukiNode
{

};

class YukiBody : public YukiStruct
{
public:
	YukiBody(YukiFileLoader* fileLoader, const YukiBlockRegion* region);

	virtual bool parse(YukiStruct* parent);
};