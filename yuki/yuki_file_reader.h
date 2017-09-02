#pragma once

class YukiFileReader
{
public:
	const YukiRegion* selectRegion(const YukiRegion* region);
	int skipBlankLines();
	const YukiRegion* cutRegionFromCursorTo(yuki_cursor cursor);
	const YukiRegion* getRegion();
	int getLineCount() const;
	const YukiLineString* getLine(int offset = 0) const;
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

protected:
	YukiFileString m_fileString;
};