#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"

static __inline int countIndentLevel(const wchar_t* str, int length, int col)
{
	int countedLength = 0;
	int indent = col;

	while (countedLength < length)
	{
		if (*str == '\t')
			indent = alignPosToTabSize(indent);
		else if (isspace(*str))
			indent++;
		else
			return indent;

		countedLength++;
		str++;
	}

	return YUKI_ERROR_INDENT;
}

yuki_line_string::yuki_line_string(yuki_file_string* parent)
	: m_parent(parent)
	, m_length(0)
	, m_contentLength(0)
	, m_indent(0)
{
}

yuki_line_string::yuki_line_string(const yuki_line_string* r)
{
	m_parent = r->m_parent;
	m_length = r->m_length;
	m_indent = r->m_indent;
	m_contentLength = r->m_contentLength;
	m_lineHeadCursor = r->m_lineHeadCursor;
	m_contentHeadCursor = r->m_contentHeadCursor;
	m_contentTailCursor = r->m_contentTailCursor;
}

bool yuki_line_string::moveCursorToNext(yuki_cursor& cursor)
{
	if (cursor.ch - m_ch == m_length - 1 || cursor.ch < m_ch)
		return false;

	wchar_t c = *getCStr();
	if (c == '\t')
	{
		int col = cursor.col + m_col;
		cursor.col += alignPosToTabSize(col) - col;
	}
	else
	{
		cursor.col++;
	}

	cursor.ch++;
	return true;
}

wchar_t yuki_line_string::getCharAtIndex(int ch) const
{
	if (ch >= getLength())
		return 0;
	return m_parent->m_buffer[ch];
}

yuki_line_string::yuki_line_string(const yuki_line_string* base, int startCol, int endCol)
{
	m_parent = base->m_parent;
	m_ln = base->m_ln;
	m_col = startCol;
	m_ch = base->getChByCol(startCol);
	m_index = base->m_index + m_ch;
	int endCh = endCol == -1 ? base->m_length - 1 : base->getChByCol(endCol);
	m_length = endCh - m_ch;
	m_ch += base->m_ch;
	
	// 统计子行的缩进
	m_indent = countIndentLevel(getCStr(), getLength(), m_col);
}

int yuki_line_string::getChByCol(int col) const
{
	col = m_col + col;

	const wchar_t* str = getCStr();
	const wchar_t* p = str;
	int cc = m_col;
	int res = 0;
	while (cc < col)
	{
		if (p - str > getLength())
			return -1;

		if (*str == '\t')
			cc = alignPosToTabSize(cc);
		else
			cc++;

		res++;
		p++;
	}

	return cc > col ? res - 1 : res;
}
