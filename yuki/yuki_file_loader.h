#pragma once

class YukiFileLoader
{
public:
	void loadYukiFile(const char* fileName);

	const YukiString* getLine(int offset = 0);
	wchar_t getChar(int offset = 0);
	inline int getLineNum() const { return m_cursor.rowNum; }

	bool moveToNextLine();
	// 查看当前行当前位置的前一个字符
	// 如果不存在前一个字符，则返回 0
	// 如果前一个字符是转义字符，则返回 1
	wchar_t peekPreviousCharInline();

	inline void saveCursor() { m_savedCursor = m_cursor; }
	inline void restoreCursor() { m_cursor = m_savedCursor; }

	bool match(const wchar_t* str);
	int matchAndSkipSpace();
	int matchIndent();
	int skipBlankLinesInRegion(const YukiBlockRegion* region);

protected:
	void splitLines();
	void initStates();

private:
	wchar_t* m_fileBuffer;
	size_t m_bufferSize;

	YukiString* m_lines;
	size_t m_lineCount;

	struct Cursor
	{
		int rowNum;
		int colNum;
		int index;			///< 当前解析光标所在的总体位置
		int indexInline;	///< 当前解析光标所在的行内位置
	} m_cursor, m_savedCursor;
};