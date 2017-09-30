#pragma once

class YukiCommentNode : public yuki_node
{

};

class YukiComment : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region);
	virtual bool match() { return true; }
};