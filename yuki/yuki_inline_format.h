#pragma once
#include "yuki_inline_markup.h"

class yuki_inline_format : public yuki_inline_markup
{
public:
	yuki_inline_format(yuki_session* globalData) : yuki_inline_markup(globalData) {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;

protected:
	virtual yuki_node* allocNode() = 0;
};