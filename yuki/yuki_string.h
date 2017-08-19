#pragma once

struct CursorOffset
{
	int colOffset;
	int indexOffset;

	CursorOffset() { colOffset = indexOffset = 0; }
};

class YukiString
{
public:
	YukiString() { m_buffer = nullptr; }

	inline size_t length() const { return m_strLength; }

	/*
		初始化一行，末尾带上 \n
	*/
	void initWithLineBuffer(wchar_t* buffer, size_t len);

	inline bool invalid() const { return m_buffer == nullptr; }

public:
	// 正则匹配函数
	int match(const wchar_t* str, int startPos) const;
	CursorOffset matchAndSkipSpace(int startPos, int colNum) const;

	bool isBlankLine() const { return m_isBlankLine; }

	bool matchQuoteBlockAttrMark();


protected:
	wchar_t* textSkipIndent();

private:
	size_t m_strLength;
	wchar_t* m_buffer;
	bool m_isBlankLine;
};

inline YukiString invalidYukiString() { return YukiString(); }