#pragma once

class YukiSubstitutionDefinitionNode : public yuki_node
{

};

class YukiSubstitutionDefinition : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
};