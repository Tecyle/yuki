#pragma once
#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

/*
	yuki_line_string
	行文本，表示 yuki 文件的一行

	行文本是由行起点游标和行长度决定的，行长度是指字符长度，而不是列长度。

	行文本会识别行的前置缩进（缩进用列数表示，只认空白字符），并且行末尾会
	去掉多余的空白字符（就算是预排文字块也会把末尾的空白字符去除）。

	注意，行文本的缩进可能和区域的缩进不一致。

	虽然行文本内部的所有游标都是绝对坐标，但是使用行文本的外部接口的时候，
	传入的游标、下标之类的应该使用相对坐标。
*/
class yuki_file_string;

class yuki_line_string
{
public:
	friend class yuki_file_string;
	friend class yuki_line_string_allocator;

	// 将光标移动到下一个位置，这里的光标是相对光标
	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLineNumber() const { return m_lineHeadCursor.ln; }
	__inline int getLength() const { return m_length; }
	__inline int getContentLength() const;
	__inline const wchar_t* getContentPtr() const;
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }
	__inline int getIndent() const { return m_indent; }
	__inline bool isSeperator() const;

	__inline yuki_cursor beginCursor() const;
	__inline yuki_cursor endCursor() const;

	__inline int getOffset() const { return m_index; }

protected:
	// yuki_file_string 专用的构造函数，通过可移动的 str 指针来构造一行
	void initByFileString(yuki_file_string* parent, int ln, wchar_t* & str);
	// YukiRegionString 专用构造函数，通过截取一段 YukiLineString 来构造一行
	void initByRegion(const yuki_line_string* base, int startCol, int endCol);

	yuki_line_string(yuki_file_string* parent);
	yuki_line_string(const yuki_line_string* r);

	// 根据列号来获取行内字符序号，特殊情况在于 Tab 字符，这里的 col 是相对列号
	int getChByCol(int col) const;

	__inline wchar_t* getCStr() const { return m_parent->m_buffer + m_index; }

private:
	yuki_file_string* m_parent;		///< 所属 file_string，字符串的源头
	yuki_cursor m_lineHeadCursor;	///< 行首所在位置的游标，绝对坐标
	int m_length;					///< 这一行的长度，不包括最后一个换行符（回车符号会被去掉）
	int m_indent;					///< 这一行的缩进级别，如果缩进级别是 YUKI_ERROR_INDENT，表明这一行是空行
};
