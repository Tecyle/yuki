#pragma once

class YukiBodyNode : public YukiNode
{

};

class YukiBody : public YukiStruct
{
public:
	YukiBody(YukiFileLoader* fileLoader, int indentLevel);

	virtual void parse(YukiStruct* parent);
};