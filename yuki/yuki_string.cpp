#include "stdafx.h"
#include <string.h>

static inline int alignPosToTabSize(int pos)
{
	int tabSize = yuki_tabSize();
	int res = (pos + tabSize) / tabSize;
	res *= tabSize;
	return res;
}

static inline bool _match(const char* &src, const char* cmp)
{
	size_t cmpLength = strlen(cmp);
	if (strncmp(src, cmp, cmpLength) == 0)
	{
		src += cmpLength;
		return true;
	}

	return false;
}

static inline int _skipWhiteSpace(const char* &src, const char* end)
{
	int spaceCount = 0;
	while (src < end)
	{
		if(!isspace(*src))
			break;

		++spaceCount;
		++src;
	}

	return spaceCount;
}

void YukiString::initWithLineBuffer(wchar_t* buffer, size_t len)
{
	m_buffer = buffer;
	m_strLength = len;

	for (int i = 0; i < (int)len; ++i)
	{
		if (!isspace(m_buffer[i]))
		{
			m_isBlankLine = false;
			return;
		}
	}

	m_isBlankLine = true;
}

int YukiString::match(const wchar_t* str, int startPos) const
{
	if (wcscmp(str, m_buffer + startPos) == 0)
		return wcslen(str);

	return 0;
}

CursorOffset YukiString::matchAndSkipSpace(int startPos, int colNum) const
{
	CursorOffset res;

	for (int i = startPos; i < length(); ++i)
	{
		wchar_t ch = m_buffer[i];

		if (ch == '\t')
		{
			int offset = alignPosToTabSize(colNum) - colNum;
			res.colOffset += offset;
			++res.indexOffset;
			colNum += offset;
		}
		else if (isspace(ch))
		{
			++res.colOffset;
			++res.indexOffset;
			++colNum;
		}
		else
		{
			break;
		}
	}

	return res;
}

bool YukiString::isBlankLine() const
{
	assert(!invalid());

	for (size_t i = 0; i < m_strLength; ++i)
	{
		if(!isspace(m_buffer[i]))
			return false;
	}

	return true;
}

/*
	匹配规则：

	* ``--`` 打头或者 ``---`` 打头
	* 中间至少要有一个空格
	* 这一行不能为空
*/
bool YukiString::matchQuoteBlockAttrMark()
{
	const char* text = textSkipIndent();

	if(!_match(text, "---") && !_match(text, "--"))
		return false;

	int spaceCount = _skipWhiteSpace(text, m_buffer + m_strLength);
	if(spaceCount == 0)
		return false;

	return text < m_buffer + m_strLength;
}
