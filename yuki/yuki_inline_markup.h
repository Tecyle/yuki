#pragma once

/*
	所有的内联标记都满足格式：

	[前缀] 文本1 [中缀] 文本2 [后缀]

	其中，中缀 和 文本2 是可以省略的。

	使用方法：
	继承本类后，按需设置前缀中缀和后缀，以及标记是否可以后跟转义符
	然后在 parse 方法中调用 match，可以获取
	m_textRegion1 : 文本1 的区间
	m_textRegion2 : 文本2 的区间
	m_endCursor : 结束位置的光标

	本类不单独使用
*/
class yuki_inline_markup : public yuki_structure_parser
{
public:
	yuki_inline_markup(yuki_session* globalData) : yuki_structure_parser(globalData) {}

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
	const wchar_t* m_suffix;

	const yuki_region* m_textRegion1;
	const yuki_region* m_textRegion2;

	yuki_cursor m_endCursor;

	bool m_allowEscapeNearMark;		///< 决定前缀后面以及后缀前面是否可以紧跟一个转义字符
};