#pragma once
#include "yuki_inline_markup.h"

class YukiInlineFormat : public YukiInlineMarkup
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;

protected:
	virtual YukiNode* allocNode() = 0;
};