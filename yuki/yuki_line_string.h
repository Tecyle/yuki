#pragma once
#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

class YukiFileString;

class YukiLineString
{
public:
	friend class YukiFileString;
	friend class YukiLinedRegionString;
	friend class YukiBlockRegionString;

	// 将光标移动到下一个位置，这里的光标是相对光标
	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLn() const { return m_ln; }
	__inline int getLength() const { return m_length; }
	__inline int getContentLength() const;
	__inline const wchar_t* getContentPtr() const;
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }
	__inline int getIndent() const { return m_indent; }
	__inline bool isSeperator() const;

	__inline yuki_cursor beginCursor() const;
	__inline yuki_cursor endCursor() const;

protected:
	// YukiFileString 专用的构造函数，通过可移动的 str 指针来构造一行
	YukiLineString(YukiFileString* parent, int ln, wchar_t* & str);
	// YukiRegionString 专用构造函数，通过截取一段 YukiLineString 来构造一行
	YukiLineString(const YukiLineString* base, int startCol, int endCol);

	YukiLineString(const YukiLineString* r);

	// 根据列号来获取行内字符序号，特殊情况在于 Tab 字符，这里的 col 是相对列号
	int getChByCol(int col) const;

	__inline wchar_t* getCStr() const { return m_parent->m_buffer + m_index; }

private:
	YukiFileString* m_parent;
	int m_ln;	///< 这一行所在的行号，全局
	int m_col;	///< 这一行的开始列号，全局
	int m_ch;	///< 这一行的行内字符序号，全局
	int m_index;	///< 这一行在整个字符串的下标位置，全局
	int m_length;	///< 这一行的长度，不包括最后一个换行符（回车符号会被去掉）
	int m_indent;	///< 这一行的缩进级别，如果缩进级别是 YUKI_ERROR_INDENT，表明这一行是空行
};
