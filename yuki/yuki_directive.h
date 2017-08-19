#pragma once

class YukiDirective
{
public:
	virtual YukiString getDirectiveName() const = 0;

	virtual bool setOption(YukiString optionName, YukiString optionValue);
	virtual bool setArgument(YukiString arg);
	virtual bool setContent(YukiString content);
};

class YukiDirectiveParser
{

};


void yukiRegisterDirective(YukiDirective* directive);