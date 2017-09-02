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
		����ĩβ�������ո񣩲������� count �� ch �ַ���
		���ƥ��ɹ�������ָ��ƥ���ַ������Ĺ�꣬
		���ƥ��ʧ�ܣ�������β��ꡣ
		ƥ�䲻���ƶ���ǰ��ꡣ
	*/
	yuki_cursor findSuffixChar(wchar_t ch, int count = 0);

protected:
	YukiFileString m_fileString;
};