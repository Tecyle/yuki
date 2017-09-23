#pragma once

class YukiSubstitutionDefinitionNode : public YukiNode
{

};

class YukiSubstitutionDefinition : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
};