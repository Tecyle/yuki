#pragma once

class YukiFileLoader
{
public:
	void loadYukiFile(const char* fileName);

	const YukiString* getLine(int offset = 0);
	wchar_t getChar(int offset = 0);
	inline int getLineNum() const { return m_cursor.rowNum; }

	bool moveToNextLine();
	// �鿴��ǰ�е�ǰλ�õ�ǰһ���ַ�
	// ���������ǰһ���ַ����򷵻� 0
	// ���ǰһ���ַ���ת���ַ����򷵻� 1
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
		int index;			///< ��ǰ����������ڵ�����λ��
		int indexInline;	///< ��ǰ����������ڵ�����λ��
	} m_cursor, m_savedCursor;
};