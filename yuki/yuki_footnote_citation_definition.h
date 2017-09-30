#pragma once

class YukiFootnoteDefinitionNode : public yuki_node
{

};

class YukiCitationDefinitionNode : public yuki_node
{

};

class YukiFootnoteCitationDefinition : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;

protected:
	bool parseLabel(wstring& label, bool& isFootnote);
};