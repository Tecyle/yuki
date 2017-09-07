#pragma once

class YukiInternalHyperlinkTargetNode : public YukiNode
{

};

class YukiExternalHyperlinkTargetNode : public YukiNode
{

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