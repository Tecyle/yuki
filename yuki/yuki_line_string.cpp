#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"

static __inline int alignPosToTabSize(int pos)
{
	int tabSize = yuki_tabSize();
	int res = (pos + tabSize) / tabSize;
	res *= tabSize;
	return res;
}

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

bool YukiLineString::moveCursorToNext(yuki_cursor& cursor)
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

wchar_t YukiLineString::getCharAtIndex(int ch) const
{
	if (ch >= getLength())
		return 0;
	return m_parent->m_buffer[ch];
}

YukiLineString::YukiLineString(YukiFileString* parent, int ln, wchar_t* & str)
{
	m_parent = parent;
	m_ln = ln;
	m_index = str - parent->m_buffer;
	m_indent = 0;
	m_col = 0;
	m_ch = 0;

	wchar_t* beginPos = str;
	bool needPassOneChar = false;
	bool countingIndent = true;
	int col = 0;
	for (;; str++)
	{
		// 遇到字符串结尾，结束搜索行
		if (*str == 0)
			break;

		// 遇到换行符，结束搜索行
		if (*str == '\n')
		{
			// 如果文档是以 ``\r\n`` 结尾的话，去掉 ``\r``
			if (str[-1] == '\r')
			{
				str--;
				*str = '\n';
				needPassOneChar = true;
			}
			break;
		}

		// 尝试统计缩进级别
		if (countingIndent)
		{
			if (!isspace(*str))
			{
				countingIndent = false;
				continue;
			}

			m_indent = *str == '\t' ? alignPosToTabSize(m_indent) : m_indent + 1;
		}
	}

	// 如果推出循环的时候都在统计缩进，表明这一行是空行
	if (countingIndent)
		m_indent = YUKI_ERROR_INDENT;

	m_length = str - beginPos;
	if (needPassOneChar)
		str++;
}

YukiLineString::YukiLineString(const YukiLineString* base, int startCol, int endCol)
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

YukiLineString::YukiLineString(const YukiLineString* r)
{
	m_parent = r->m_parent;
	m_ln = r->m_ln;
	m_col = r->m_col;
	m_ch = r->m_ch;
	m_index = r->m_index;
	m_length = r->m_length;
	m_indent = r->m_indent;
}

int YukiLineString::getChByCol(int col) const
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
