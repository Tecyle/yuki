#pragma once

enum yuki_hyperlink_target_type
{
	HyperlinkTarget_internal,
	HyperlinkTarget_external,
	HyperlinkTarget_alias
};

class YukiHyperlinkTargetNode : public yuki_node
{
public:
	void setTargetType(yuki_hyperlink_target_type type);
	wstring& uri();
};

class YukiHyperlinkTarget : public yuki_structure
{
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