#pragma once

class YukiSubstitutionDefinitionNode : public YukiNode
{

};

class YukiSubstitutionDefinition : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
};