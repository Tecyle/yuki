#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_line_string.h"
#include "yuki_file_string.h"
#include "yuki_object_allocator.h"
#include "yuki_line_string_manager.h"

typedef yuki_object_allocator<yuki_line_string, yuki_file_string*> yuki_line_string_allocator;

yuki_line_string_manager::yuki_line_string_manager()
	: m_allocator(new yuki_line_string_allocator())
{
}

yuki_line_string_manager::~yuki_line_string_manager()
{
	delete m_allocator;
}

const yuki_line_string* yuki_line_string_manager::allocLineStringForFileString(
	yuki_file_string* fileString, int ln, wchar_t* & str)
{
	yuki_line_string* line = allocLineString(fileString);
	yuki_cursor& cursor = line->m_lineHeadCursor;
	cursor.setCursor(ln, 0, str - fileString->getBuffer(), 0);
	line->m_indent = 0;

	wchar_t* beginPos = str;
	bool needPassOneChar = false;
	bool countingIndent = true;
	// �����������ó���������α�
	wchar_t ch = *str;
	str++;
	int col = 0;
	int chCount = 0;
	while (!yukiIsLineBreak(ch) && isspace(ch))
	{
		col = ch == '\t' ? yukiAlignPosToTabSize(col) : col + 1;
		chCount++;
		ch = *str;
		str++;
	}
	// ��һ��ֻ�пհ��ַ���˵���ǿ���
	if (yukiIsLineBreak(ch) || ch == 0)
	{
		line->m_indent = YUKI_ERROR_INDENT;
		line->m_length = 0;
		return line;
	}

	cursor = line->m_contentHeadCursor;
	cursor.setCursor(ln, col, str - fileString->getBuffer(), chCount);
	line->m_indent = col + 1;

	// �����������ݳ���
	cursor = line->m_contentTailCursor;
	cursor.setCursor(ln, col, str - fileString->getBuffer(), chCount);
	bool isLastCharSpace = false;
	// �����ѭ���϶����ٻ�ִ��һ��
	for (; ch != 0 && !yukiIsLineBreak(ch); str++)
	{
		ch = *str;
		// �����հ��ַ�����¼λ�ã��˴���������β�հ�
		if (!isLastCharSpace && isspace(ch))
		{
			cursor.col = col;
			cursor.offset = str - fileString->getBuffer();
			cursor.ch = chCount;
			isLastCharSpace = true;
		}

		col = ch == '\t' ? yukiAlignPosToTabSize(col) : col + 1;
		chCount++;
	}
	// �ж������յ��α��Ƿ���ȷ����
	if (!isLastCharSpace)
	{
		cursor.col = col;
		cursor.offset = str - fileString->getBuffer();
		cursor.ch = chCount;
	}

	// ���ÿ��
	line->m_length = str - beginPos;
	line->m_contentLength = line->m_contentTailCursor.ch - line->m_contentHeadCursor.ch;
	if (ch == '\n' && str[-1] == '\r')
		line->m_length--;

	str++;
	return line;
}

const yuki_line_string* yuki_line_string_manager::cloneLineString(const yuki_line_string* str)
{
	return m_allocator->cloneObject(*str);
}

void yuki_line_string_manager::freeLineString(const yuki_line_string* str)
{
	return m_allocator->freeObject(const_cast<yuki_line_string*>(str));
}

yuki_line_string* yuki_line_string_manager::allocLineString(yuki_file_string* fileString)
{
	return m_allocator->allocObject(fileString);
}
