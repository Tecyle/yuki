#pragma once

class YukiFileReader
{
public:
	const yuki_region* selectRegion(const yuki_region* region);
	int skipBlankLines();
	const yuki_region* cutRegionFromCursorTo(yuki_cursor cursor, int indent = -1);
	const yuki_region* cutRegionFromCursorToEnd();
	// ����ṩ�� indent����ʹ�����ֵ��Ϊ indent������� -1�����Զ����� indent
	const yuki_region* cutRegionToCursorFrom(yuki_cursor cursor, int indent = -1);
	const yuki_region* cutRegionBetween(yuki_cursor start, yuki_cursor end, int indent = -1);
	const yuki_region* getRegion();
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

	// �ú������������������
	bool moveToNextChar(bool crossLines = true);
	wchar_t getChar(int offset = 0);
	wchar_t peekPreviousChar();

	bool searchStr(const wchar_t* str, bool crossLine = false);
	bool matchBlankLine();

protected:
	yuki_file_string m_fileString;
};