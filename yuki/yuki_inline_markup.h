#pragma once

/*
	所有的内联标记都满足格式：

	[前缀] 文本1 [中缀] 文本2 [后缀]

	其中，中缀 和 文本2 是可以省略的。
*/
class YukiInlineMarkup : public YukiStruct
{
public:
	virtual bool parse(YukiStruct* parent);

protected:
	virtual bool matchPrefix();
	virtual bool searchInfix();
	virtual bool searchSufix();

protected:
	const wchar_t* m_prefix;
	const wchar_t* m_infix;
	const wchar_t* m_sufix;

	YukiString m_text1;
	YukiString m_text2;

	bool m_allowEscapeNearMark;		///< 决定前缀后面以及后缀前面是否可以紧跟一个转义字符
};