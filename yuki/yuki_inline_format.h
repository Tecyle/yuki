#pragma once
#include "yuki_inline_markup.h"

class YukiInlineFormat : public YukiInlineMarkup
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;

protected:
	virtual yuki_node* allocNode() = 0;
};