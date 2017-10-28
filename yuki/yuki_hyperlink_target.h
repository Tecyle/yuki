#pragma once

class yuki_hyperlink_target : public yuki_structure_parser
{
public:
	yuki_hyperlink_target(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_hyperlink_target() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

private:
	bool matchNoBackward();
	bool readTargetName(wchar_t endChar);

private:
	wstring m_matchedName;
	bool m_anonymousName;
};