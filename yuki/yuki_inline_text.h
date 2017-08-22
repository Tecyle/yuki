#pragma once

enum YukiInlineTextType
{
	Yuki_normal,
	Yuki_reserveAllText,
};

class YukiInlineText : public YukiStruct
{
public:

protected:
	virtual bool parseLineRegionNormalText();
	virtual bool parseLineRegionReserveText();
	virtual bool parseNormalText();

protected:
	wstring m_inlineText;
};