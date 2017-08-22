#include "stdafx.h"
#include "yuki.h"
#include "yuki_inline_text.h"

/*
	�����ı�������

	* ��������ת���ַ���ת��ո�����κζ���
	* �س�������һ���س������س�����հ��ַ�
	* ����հ��ַ�������һ���ո�
	* ÿ���������У�����ָ������
*/
bool YukiInlineText::parseNormalText()
{

}

bool YukiInlineText::parseLineRegionNormalText()
{
	assert(m_limitRegion->type == Yuki_lineRegion);

	int startPos = m_limitRegion->startIndex;
	bool lastCharIsSpace = false;
	for (int lineNum = m_limitRegion->startLineNum; !m_limitRegion->isLineOutOfRegion(lineNum); ++lineNum)
	{
		const YukiString* line = m_fileLoader->getLineByIndex(lineNum);
		assert(line != nullptr);

		if (line->isBlankLine())
		{
			lastCharIsSpace = true;
			continue;
		}

		// �������ڽ���
		while (startPos < line->length())
		{
			// ��������
			if (startPos == 0)
			{
				CursorOffset indent = line->getIndentLevel();
				assert(indent.colOffset >= m_limitRegion->indent);
				startPos = indent.indexOffset;
				if (m_limitRegion->indent < indent.colOffset)
					lastCharIsSpace = true;
			}

			wchar_t curChar = line->at(startPos);
			++startPos;

			if (curChar == '\r')
			{
				// ���Իس���
				assert(startPos == line->length() - 2);
			}
			else if (isspace(curChar))
			{
				// ����հ��ַ���ֻ�����ɵ����Ŀո�
				lastCharIsSpace = true;
			}
			else
			{
				if (lastCharIsSpace)
					m_inlineText += ' ';
				lastCharIsSpace = false;

				if (curChar == '\\')
				{
					// �����ת���ַ�������ת��
					assert(!outOfRegion());
					curChar = line->at(startPos++);
					if (curChar != ' ' && curChar != '\r' && curChar != '\n')
						m_inlineText += curChar;
				}
				else
				{
					// �����ַ���ֱ���ƽ�
					m_inlineText += curChar;
				}
			}
		}

		// �ƶ�����һ������
		startPos = 0;
	}

	return true;
}

/*
	Ԥ���ı�������

	* ��������ת�壬�������пհ�
	* �س�������һ���س������س�������
	* ÿ���������У�����ָ������
*/
bool YukiInlineText::parseLineRegionReserveText()
{
	assert(m_limitRegion->type == Yuki_lineRegion);

	int startPos = m_limitRegion->startIndex;
	for (int lineNum = m_limitRegion->startLineNum; !m_limitRegion->isLineOutOfRegion(lineNum); ++lineNum)
	{
		const YukiString* line = m_fileLoader->getLineByIndex(lineNum);
		assert(line != nullptr);

		// �������ڽ���
		while (startPos < line->length())
		{
			// ��������
			if (startPos == 0)
			{
				CursorOffset indent = line->getIndentLevel();
				assert(indent.colOffset >= m_limitRegion->indent);
				startPos = indent.indexOffset;
				if (m_limitRegion->indent < indent.colOffset)
				{
					while (indent.colOffset < m_limitRegion->indent)
					{
						m_inlineText += ' ';
						--indent.colOffset;
					}
				}
			}

			wchar_t curChar = line->at(startPos);
			++startPos;

			if (curChar == '\r')
			{
				// ���Իس���
				assert(startPos == line->length() - 2);
			}
			else
			{	// �����ַ���ֱ���ƽ�
				m_inlineText += curChar;
			}
		}

		// �ƶ�����һ������
		startPos = 0;
	}

	return true;
}
