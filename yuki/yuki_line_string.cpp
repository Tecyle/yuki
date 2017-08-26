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

bool YukiLineString::moveCursorToNext(yuki_cursor& cursor)
{
	if (cursor.ch == m_length - 1)
		return false;

	wchar_t c = m_parent->get
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

YukiLineString::YukiLineString(YukiLineString* base, int startCol, int endCol)
{

}

int YukiLineString::getChByCol(int col)
{

}
