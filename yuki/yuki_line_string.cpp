#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_region_string.h"
#include "yuki_file_string.h"

yuki_line_string::yuki_line_string(yuki_file_string* parent)
	: m_parent(parent)
{
}

yuki_line_string::yuki_line_string(const yuki_line_string* r)
{
	m_parent = r->m_parent;
	m_lineHeadCursor = r->m_lineHeadCursor;
	m_lineTailCursor = r->m_lineTailCursor;
	m_contentHeadCursor = r->m_contentHeadCursor;
	m_contentTailCursor = r->m_contentTailCursor;
}

bool yuki_line_string::moveCursorToNext(yuki_cursor& cursor) const
{
	if (!isCursorInLine(cursor))
		return false;

	wchar_t c = getCharAtCursor(cursor);
	cursor.col = c == '\t' ? yukiAlignPosToTabSize(cursor.col, yuki_tabSize()) : cursor.col + 1;
	cursor.ch++;
	cursor.offset++;
	return true;
}

bool yuki_line_string::isCursorInLine(const yuki_cursor& cursor) const
{
	return cursor >= m_lineHeadCursor && cursor < m_lineTailCursor;
}

wchar_t yuki_line_string::getCharAtIndex(int ch) const
{
	if (ch >= getLength())
		return 0;
	return m_parent->m_buffer[ch];
}

bool yuki_line_string::isCursorAtLineEnd(const yuki_cursor& cursor) const
{
	return cursor >= m_contentTailCursor && cursor <= m_lineTailCursor;
}

yuki_cursor yuki_line_string::getCursorByCol(int colNum) const
{
	yuki_cursor cursor;
	int ch = getChByCol(colNum);

	if (ch < 0)
		return cursor;

	cursor.setCursor(m_lineHeadCursor.ln, colNum, m_lineHeadCursor.offset + ch, ch);
	return cursor;
}

wchar_t yuki_line_string::getCharAtCursor(const yuki_cursor& cursor) const
{
	if (cursor.ln != m_lineHeadCursor.ln)
		return 0;

	if (cursor.ch > m_contentTailCursor.ch)
		return 0;

	return m_parent->getBuffer()[cursor.offset];
}

const wchar_t* yuki_line_string::getCStr(int ch) const
{
	if (ch > m_contentTailCursor.ch)
		return nullptr;

	return m_parent->getBuffer() + ch;
}

int yuki_line_string::getChByCol(int col) const
{
	const wchar_t* str;
	int _col;
	int ch;

	if (col > m_contentTailCursor.col)
		return -1;

	if (col >= m_contentHeadCursor.col)
	{
		str = m_parent->getBuffer() + m_contentHeadCursor.offset;
		_col = m_contentHeadCursor.col;
		ch = m_contentHeadCursor.ch;
	}
	else
	{
		str = m_parent->getBuffer() + m_lineHeadCursor.offset;
		_col = m_lineHeadCursor.col;
		ch = m_lineHeadCursor.ch;
	}
	
	while (_col < col)
	{
		wchar_t c = *str++;
		ch++;
		_col = c == '\t' ? yukiAlignPosToTabSize(_col, yuki_tabSize()) : _col + 1;
	}

	return ch;
}
