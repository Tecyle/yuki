#include "stdafx.h"
#include "yuki.h"
#include "yuki_inline_text.h"

/*
	常规文本解析：

	* 正常解析转义字符，转义空格不输出任何东西
	* 回车换行算一个回车符，回车符算空白字符
	* 多个空白字符解析成一个空格
	* 每次遇到新行，跳过指定缩进
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

		// 进行行内解析
		while (startPos < line->length())
		{
			// 跳过缩进
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
				// 忽略回车符
				assert(startPos == line->length() - 2);
			}
			else if (isspace(curChar))
			{
				// 多个空白字符，只解析成单个的空格
				lastCharIsSpace = true;
			}
			else
			{
				if (lastCharIsSpace)
					m_inlineText += ' ';
				lastCharIsSpace = false;

				if (curChar == '\\')
				{
					// 如果是转义字符，进行转义
					assert(!outOfRegion());
					curChar = line->at(startPos++);
					if (curChar != ' ' && curChar != '\r' && curChar != '\n')
						m_inlineText += curChar;
				}
				else
				{
					// 其它字符，直接移进
					m_inlineText += curChar;
				}
			}
		}

		// 移动到下一行行首
		startPos = 0;
	}

	return true;
}

/*
	预排文本解析：

	* 忽略所有转义，保留所有空白
	* 回车换行算一个回车符，回车符保留
	* 每次遇到新行，跳过指定缩进
*/
bool YukiInlineText::parseLineRegionReserveText()
{
	assert(m_limitRegion->type == Yuki_lineRegion);

	int startPos = m_limitRegion->startIndex;
	for (int lineNum = m_limitRegion->startLineNum; !m_limitRegion->isLineOutOfRegion(lineNum); ++lineNum)
	{
		const YukiString* line = m_fileLoader->getLineByIndex(lineNum);
		assert(line != nullptr);

		// 进行行内解析
		while (startPos < line->length())
		{
			// 跳过缩进
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
				// 忽略回车符
				assert(startPos == line->length() - 2);
			}
			else
			{	// 其它字符，直接移进
				m_inlineText += curChar;
			}
		}

		// 移动到下一行行首
		startPos = 0;
	}

	return true;
}
