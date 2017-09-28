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

/*
	����һ�� line string��ֻ��Ҫȷ���ĸ� cursor ��λ�ü��ɡ�
*/
const yuki_line_string* yuki_line_string_manager::allocLineStringForFileString(
	yuki_file_string* fileString, int ln, wchar_t* & str)
{
	assert(ln < fileString->getLineCount());

	yuki_line_string* line = allocLineString(fileString);
	yuki_cursor& cursor = line->m_lineHeadCursor;
	cursor.setCursor(ln, 0, str - fileString->getBuffer(), 0);

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
		line->m_lineTailCursor = line->m_lineHeadCursor;
		line->m_contentHeadCursor = line->m_lineHeadCursor;
		line->m_contentTailCursor = line->m_contentHeadCursor;
		return line;
	}

	cursor = line->m_contentHeadCursor;
	cursor.setCursor(ln, col, str - fileString->getBuffer(), chCount);

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

	// �����յ��α�
	cursor = line->m_lineTailCursor;
	cursor.col = col;
	cursor.offset = str - fileString->getBuffer();
	cursor.ch = chCount;
	if (str[-1] == '\r')
	{
		cursor.col--;
		cursor.offset--;
		cursor.ch--;
	}

	str++;
	return line;
}

/*
	�ü� line string ���߼���
	
	1. region ���������������

		1. �ڶ��п�ʼ�������У����ԭʼ�������� region ����������ʹ��ԭʼ
		   ���������򣬴� region ������λ�ÿ�ʼ�������հ��ַ�����������
		2. ��β��ԭʼ����βһ�£��ü����յ��αꡣ
		3. ��������У�������λ��С�ڵ��� region ����������������б���
		   һ�����߼���������λ�ô��� region ������������� region ����
		   ����

	2. region ����ǿ���������

		1. �ڶ��п�ʼ�������н�ȡ leftEdge �� rightEdge ֮������ݣ����ԭ
		   ʼ�������� region ������ + leftEdge ���кţ���ʹ��ԭʼ����������
		   ���Ǹ�λ�ÿ�ʼ�����հ��ַ�����������
		2. ��������У�������������������һ�£�ֻ����Ҫ�޶��� leftEdge ��
		   rightEdge ֮�䡣
		3. �����β�У��ü����յ��αꡣ
*/
const yuki_line_string* yuki_line_string_manager::allocLineStringForRegionString(yuki_file_string* fileString, 
	const yuki_region* region, int lnInRegion)
{
	// ����к��Ƿ�Խ��
	const yuki_cursor& endPos = region->end();
	// ��� endCursor ��λ��ĳ�е����ף�Ӧ�ò�������һ�У��ϲ��߼���֤���
	if (endPos.isValid() && lnInRegion > endPos.ln)
		return nullptr;
	if (!endPos.isValid() && lnInRegion >= fileString->getLineCount())
		return nullptr;

	// ��ʼ���� line string
	yuki_line_string* line = allocLineString(fileString);
	const yuki_line_string* originalLine = fileString->getLine(region->begin().ln + lnInRegion);
	assert(originalLine);

	if (lnInRegion == region->begin().ln)
	{
		// ��������
		line->m_lineHeadCursor = region->begin();
		if (region->getRegionType() == Yuki_linedRegion)
		{
			line->m_lineTailCursor = originalLine->m_lineTailCursor;
		}
		else
		{
			line->m_lineTailCursor = region->getRightEdige();
		}
	}
	else if ((!region->end().isValid() && lnInRegion < fileString->getLineCount())
		|| lnInRegion < region->end().ln)
	{
		// �����м���
		if (region->getRegionType() == Yuki_linedRegion)
		{
			line->m_lineHeadCursor = originalLine->m_lineHeadCursor;
			line->m_lineTailCursor = originalLine->m_lineTailCursor;
		}
		else
		{
			line->m_lineHeadCursor = region->getLeftEdgeCursor();
			line->m_lineTailCursor = region->getRightEdige();
		}
	}
	else
	{
		// ����β��
		line->m_lineTailCursor = region->end();
		if (region->getRegionType() == Yuki_linedRegion)
		{
			line->m_lineHeadCursor = originalLine->m_lineHeadCursor;
		}
		else
		{
			line->m_lineHeadCursor = region->getLeftEdgeCursor();
		}
	}
	// content tail
	if (line->m_lineTailCursor < originalLine->m_contentTailCursor)
		line->m_contentTailCursor = line->m_lineTailCursor;
	else
		line->m_contentTailCursor = originalLine->m_contentTailCursor;
	// content head
	if (originalLine->getIndent() >= region->getIndent())
	{
		line->m_contentHeadCursor = originalLine->m_contentHeadCursor;
	}
	else
	{
		yuki_cursor& cursor = line->m_contentHeadCursor;
		cursor = originalLine->getCursorByCol(region->getIndent());
		wchar_t ch = originalLine->getCharAtCursor(cursor);
		while (isspace(ch))
		{
			cursor.col = ch == '\t' ? yukiAlignPosToTabSize(cursor.col) : cursor.col + 1;
			cursor.ch++;
			cursor.offset++;
			ch = originalLine->getCharAtCursor(cursor);
		}
	}
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
