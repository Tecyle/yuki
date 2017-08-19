#pragma once

class YukiFileLoader
{
public:
	void loadYukiFile(const char* fileName);

	const YukiString* getLine(int offset = 0);
	inline int getLineNum() const { return m_cursor.rowNum; }

	bool moveToNextLine();

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