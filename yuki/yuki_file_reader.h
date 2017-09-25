#pragma once

class YukiFileReader
{
public:
	const yuki_region* selectRegion(const yuki_region* region);
	int skipBlankLines();
	const yuki_region* cutRegionFromCursorTo(yuki_cursor cursor, int indent = -1);
	const yuki_region* cutRegionFromCursorToEnd();
	// 如果提供了 indent，则使用这个值作为 indent，如果是 -1，则自动计算 indent
	const yuki_region* cutRegionToCursorFrom(yuki_cursor cursor, int indent = -1);
	const yuki_region* cutRegionBetween(yuki_cursor start, yuki_cursor end, int indent = -1);
	const yuki_region* getRegion();
	int getLineCount() const;
	const yuki_line_string* getLine(int offset = 0) const;
	yuki_cursor getCursor() const;
	void setCursor(yuki_cursor cursor);
	bool matchStr(const wchar_t* str);
	bool matchChar(wchar_t ch);
	int skipSpaces();
	bool cursorAtLineEnd() const;
	bool moveToNextLine();
	/* 
		从行末尾（跳过空格）查找连续 count 个 ch 字符，
		如果匹配成功，返回指向匹配字符串起点的光标，
		如果匹配失败，返回行尾光标。
		匹配不会移动当前光标。
	*/
	yuki_cursor findSuffixChar(wchar_t ch, int count = 0);

	// 该函数会跳过区域的缩进
	bool moveToNextChar(bool crossLines = true);
	wchar_t getChar(int offset = 0);
	wchar_t peekPreviousChar();

	bool searchStr(const wchar_t* str, bool crossLine = false);
	bool matchBlankLine();

protected:
	yuki_file_string m_fileString;
};