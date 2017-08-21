#pragma once

struct CursorOffset
{
	int colOffset;
	int indexOffset;

	CursorOffset() { colOffset = indexOffset = 0; }
	CursorOffset& operator+=(int r) {
		colOffset += r;
		indexOffset += r;
		return *this;
	}
};

class YukiString
{
public:
	YukiString() { m_buffer = nullptr; }

	inline size_t length() const { return m_strLength; }

	/*
		��ʼ��һ�У�ĩβ���� \n
	*/
	void init(wchar_t* buffer, size_t len);

	inline bool invalid() const { return m_buffer == nullptr; }

	const wchar_t* begin() const { return m_buffer; }
	const wchar_t* end() const { return m_buffer + m_strLength; }

public:
	// ����ƥ�亯����YukiString �е�ƥ�亯�������ƶ���ǰ��꣬�൱�� peek
	int match(const wchar_t* str, int startPos) const;
	CursorOffset matchAndSkipSpace(int startPos, int colNum) const;
	inline CursorOffset matchAndSkipSpace(CursorOffset offset) const { return matchAndSkipSpace(offset.indexOffset, offset.indexOffset); }

	CursorOffset getIndentLevel() const;

	bool isBlankLine() const { return m_isBlankLine; }
	bool matchNoEmptyChar(int startPos) const;

	bool matchQuoteBlockAttrMark() const;

	CursorOffset moveSingleChar(int startPos, int colNum) const;


protected:
	wchar_t* textSkipIndent();

private:
	size_t m_strLength;
	wchar_t* m_buffer;
	bool m_isBlankLine;
};

inline YukiString invalidYukiString() { return YukiString(); }