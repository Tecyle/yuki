#pragma once

/*
	所有的内联标记都满足格式：

	[前缀] 文本1 [中缀] 文本2 [后缀]

	其中，中缀 和 文本2 是可以省略的。
*/
class YukiInlineMarkup : public YukiStruct
{
public:
	virtual bool match() override;

protected:
	virtual bool matchPrefix();
	virtual bool matchInfix(yuki_cursor& cursor);
	virtual bool matchSuffix(yuki_cursor& cursor);

	virtual bool searchAndMatch(const wchar_t* pattern, yuki_cursor& contentCursor);

protected:
	const wchar_t* m_prefix;
	const wchar_t* m_infix;
	const wchar_t* m_sufix;

	const YukiRegion* m_textRegion1;
	const YukiRegion* m_textRegion2;

	yuki_cursor m_endCursor;

	bool m_allowEscapeNearMark;		///< 决定前缀后面以及后缀前面是否可以紧跟一个转义字符
};