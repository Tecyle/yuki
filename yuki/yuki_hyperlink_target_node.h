#pragma once

enum yuki_hyperlink_target_type
{
	HyperlinkTarget_internal,
	HyperlinkTarget_external,
	HyperlinkTarget_alias
};

class yuki_hyperlink_target_node : public yuki_node
{
public:
	void setTargetType(yuki_hyperlink_target_type type);
	wstring& uri();
};
