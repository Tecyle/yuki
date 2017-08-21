#pragma once

class YukiBodyNode : public YukiNode
{

};

class YukiBody : public YukiStruct
{
public:
	YukiBody(YukiFileLoader* fileLoader, int indentLevel);

	virtual bool parse(YukiStruct* parent);
};