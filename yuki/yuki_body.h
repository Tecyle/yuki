#pragma once

class YukiBodyNode : public yuki_node
{

};

class YukiBody : public yuki_structure
{
public:
	YukiBody(yuki_session* globalData);

public:
	virtual bool parse(yuki_node* parent, const yuki_region* region) override;
};