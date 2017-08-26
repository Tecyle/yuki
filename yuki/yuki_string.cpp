#include "stdafx.h"
#include <string.h>

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

void YukiString::init(wchar_t* buffer, size_t len)
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

CursorOffset YukiString::getIndentLevel() const
{
	if (isBlankLine())
		return CursorOffset();

	return matchAndSkipSpace(0, 0);
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

bool YukiString::matchNoEmptyChar(int startPos) const
{
	const wchar_t* pos = begin() + startPos;

	if(pos >= end())
		return false;

	return !isspace(*pos);
}

/*
	ƥ�����

	* ``--`` ��ͷ���� ``---`` ��ͷ
	* �м�����Ҫ��һ���ո�
	* ��һ�в���Ϊ��
*/
bool YukiString::matchQuoteBlockAttrMark() const
{
	CursorOffset nowPos = getIndentLevel();
	if(nowPos.indexOffset < 0)
		return false;

	// �ȳ���ƥ�� ``---``��������ɹ��ٳ���ƥ�� ``--``����������ɹ��򷵻� false
	int offset = match(L"---", nowPos.indexOffset);
	if (offset == 0)
		offset = match(L"--", nowPos.indexOffset);
	if (offset == 0)
		return false;

	nowPos += offset;
	// ��Ҫƥ�䲢�����ո񣬿ո���������Ϊ 1
	CursorOffset cOffset = matchAndSkipSpace(nowPos);
	if(cOffset.indexOffset == 0)
		return false;

	return matchNoEmptyChar(nowPos.indexOffset + cOffset.indexOffset);
}

CursorOffset YukiString::moveSingleChar(int startPos, int colNum) const
{
	CursorOffset res;

	if (startPos >= m_strLength - 1)
		return res;

	wchar_t ch = m_buffer[startPos];
	int offset = 1;

	if (ch == '\t')
	{
		offset = alignPosToTabSize(colNum) - colNum;
	}

	res.colOffset = colNum + offset;
	res.indexOffset = startPos + 1;

	return res;
}
