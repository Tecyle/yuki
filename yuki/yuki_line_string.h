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
	friend class yuki_line_string_manager;

	// 将光标移动到下一个位置，这里的光标是相对光标
	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLineNumber() const { return m_lineHeadCursor.ln; }
	__inline int getLength() const { return m_lineTailCursor.ch - m_lineHeadCursor.ch; }
	__inline int getContentLength() const { return m_contentTailCursor.ch - m_contentHeadCursor.ch; }
	__inline const wchar_t* getContentPtr() const;
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return getContentLength() == 0; }
	__inline int getIndent() const { return m_contentHeadCursor.col - m_lineHeadCursor.col; }
	__inline bool isSeperator() const;

	__inline const yuki_cursor& beginCursor() const { return m_lineHeadCursor; }
	__inline const yuki_cursor& endCursor() const { return m_contentTailCursor; }
	__inline const yuki_cursor& contentBeginCursor() const { return m_contentHeadCursor; }
	__inline const yuki_cursor& contentEndCursor() const { return m_contentTailCursor; }

	__inline int getOffset() const { return m_lineHeadCursor.offset; }

	/*
		根据列号返回这个列号在这一行内的位置游标。
		列号出错的时候返回无效游标。
	*/
	yuki_cursor getCursorByCol(int colNum) const;

	/*
		获取游标所在位置的字符，任何不匹配都将返回 0。
	*/
	wchar_t getCharAtCursor(const yuki_cursor& cursor) const;

protected:
	yuki_line_string(yuki_file_string* parent);
	yuki_line_string(const yuki_line_string* r);

	// 根据列号来获取行内字符序号，特殊情况在于 Tab 字符，这里的 col 是相对列号
	int getChByCol(int col) const;

	__inline wchar_t* getCStr() const { return m_parent->m_buffer + getOffset(); }

private:
	yuki_file_string* m_parent;			///< 所属 file_string，字符串的源头
	yuki_cursor m_lineHeadCursor;		///< 行首所在位置的游标，绝对坐标，包含该位置
	yuki_cursor m_lineTailCursor;		///< 行尾所在位置的游标，绝对坐标，不包含该位置
	yuki_cursor m_contentHeadCursor;	///< 内容部分开始的游标，绝对坐标，包含该位置
	yuki_cursor m_contentTailCursor;	///< 内容部分结束的游标，绝对坐标，不包含该位置
};
