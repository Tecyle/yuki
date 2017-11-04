#pragma once

class yuki_inline_bold_italic : public yuki_inline_format
{
public:
	yuki_inline_bold_italic(yuki_session* globalData);

protected:
	virtual yuki_node* allocNode() override;
};