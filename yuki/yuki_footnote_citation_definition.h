#pragma once

class YukiFootnoteDefinitionNode : public YukiNode
{

};

class YukiCitationDefinitionNode : public YukiNode
{

};

class YukiFootnoteCitationDefinition : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;

protected:
	bool parseLabel(wstring& label, bool& isFootnote);
};