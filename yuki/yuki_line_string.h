#pragma once
#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

class YukiFileString;

class YukiLineString
{
public:
	friend class YukiFileString;

	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLn() const { return m_ln; }
	__inline int getLength() const { return m_length; }
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }

protected:
	// YukiFileString 专用的构造函数，通过可移动的 str 指针来构造一行
	YukiLineString(YukiFileString* parent, int ln, wchar_t* & str);
	// YukiRegionString 专用构造函数，通过截取一段 YukiLineString 来构造一行
	YukiLineString(YukiLineString* base, int startCol, int endCol);

	// 根据列号来获取行内字符序号，特殊情况在于 Tab 字符
	int getChByCol(int col);

private:
	YukiFileString* m_parent;
	int m_ln;	///< 这一行所在的行号
	int m_index;	///< 这一行在整个字符串的下标位置
	int m_length;	///< 这一行的长度，不包括最后一个换行符（回车符号会被去掉）
	int m_indent;	///< 这一行的缩进级别，如果缩进级别是 YUKI_ERROR_INDENT，表明这一行是空行
};
