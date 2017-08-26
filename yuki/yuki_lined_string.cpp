#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"

#include "yuki_lined_string.h"

static int countLines(const wchar_t* str)
{
	int res = 1;

	while (*str)
	{
		if (*str == '\n')
			res++;
	}

	return res;
}

static __inline int alignPosToTabSize(int pos)
{
	int tabSize = yuki_tabSize();
	int res = (pos + tabSize) / tabSize;
	res *= tabSize;
	return res;
}

class YukiLinedRegionString : public YukiRegionString
{
public:
	YukiLinedRegionString(YukiLinedString* parent, int sLn, int sCol, int eLn, int eCol, int sCh, int indent) 
		: YukiRegionString(parent)
	{
		m_region = new YukiLinedRegion(sLn, sCol, eLn, eCol, sCh);
		
		assert(indent != YUKI_ERROR_INDENT);
		m_region->setIndent(indent);

		m_lineCount = eCol == -1 ? parent->getLineCount() : eCol;
		m_lineCount -= sLn;
		m_lines = parent->getLine(sLn);
	}

	~YukiLinedRegionString() { delete m_region; }



// 	const YukiRegion* calcRegionBySubregion(const YukiRegion* subregion)
// 	{
// 		// 行区域套行区域还是行区域，行区域套块区域是块区域
// 		if (subregion->getRegionType() == Yuki_linedRegion)
// 			return new YukiLinedRegion(
// 				m_region->getSLn() + subregion->getSLn(),
// 				m_region->getSCol() + subregion->getSCol(),
// 				m_region->getSLn() + subregion->getELn(),
// 				m_region->getSCol() + subregion->getECol(),
// 				calc
// 			)
// 	}

private:
	const YukiStringLine* m_lines;
	int m_lineCount;
};

YukiLinedString::YukiLinedString()
{
	m_buffer = nullptr;
	m_lines = nullptr;
	m_bufferLength = 0;
	m_lineCount = 0;
}

YukiLinedString::~YukiLinedString()
{
	free(m_buffer);
	free(m_lines);
}

const YukiStringLine* YukiLinedString::getLine(int index)
{
	if (m_lineCount == 0)
		return nullptr;

	index = yuki_get_absolute_index(index, m_lineCount);
	if (index >= 0 && index < m_lineCount)
		return m_lines + index;

	return nullptr;
}

bool YukiLinedString::buildFromBuffer()
{
	assert(m_buffer != nullptr);

	free(m_lines);

	m_lineCount = countLines(m_buffer);
	m_lines = (YukiLinedString*)malloc(m_lineCount * sizeof(YukiLinedString));
	
	wchar_t* p = m_buffer;
	for (int i = 0; i < m_lineCount; ++i)
	{
		YukiStringLine* line = m_lines + i;
		line->YukiStringLine::YukiStringLine(this, i, p);
	}

	assert(*p == 0);
	return true;
}

wchar_t YukiStringLine::getCharAtIndex(int ch) const
{
	if (ch >= getLength())
		return 0;
	return m_parent->m_buffer[ch];
}

YukiStringLine::YukiStringLine(YukiLinedString* parent, int ln, wchar_t* & str)
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
