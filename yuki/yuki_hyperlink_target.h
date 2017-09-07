#pragma once

enum yuki_hyperlink_target_type
{
	HyperlinkTarget_internal,
	HyperlinkTarget_external,
	HyperlinkTarget_alias
};

class YukiHyperlinkTargetNode : public YukiNode
{
public:
	void setTargetType(yuki_hyperlink_target_type type);
	wstring& uri();
};

class YukiHyperlinkTarget : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

private:
	bool matchNoBackward();
	bool readTargetName(wchar_t endChar);

private:
	wstring m_matchedName;
	bool m_anonymousName;
};