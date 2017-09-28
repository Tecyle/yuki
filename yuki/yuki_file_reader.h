#pragma once

class yuki_region_stack_manager;

/*
	yuki_file_reader
	yuki �ļ���ȡ��

	ע�⣬��ʹ�õ� cursor ���Ǿ���λ���α꣬������Ҫȥ�Լ����� cursor��
*/
class yuki_file_reader
{
public:
	yuki_file_reader();
	~yuki_file_reader();

public:
	bool openFile(const wchar_t* fileName);

	/*
		�� region ѡ��Ϊ��ǰ��Ч�� region����ѹ�� region ջ��
	*/
	__inline void pushRegion(const yuki_region* region);
	/*
		������ǰ region�����ָ�֮ǰ�� region��
	*/
	__inline void popRegion();

	/*
		�������У�����������������
		���û�����κο��У����� 0��
	*/
	int skipBlankLines();
	const yuki_region* cutRegionFromCursorTo(const yuki_cursor& cursor, int indent = -1);
	const yuki_region* cutRegionFromCursorToEnd();
	// ����ṩ�� indent����ʹ�����ֵ��Ϊ indent������� -1�����Զ����� indent
	const yuki_region* cutRegionToCursorFrom(const yuki_cursor& cursor, int indent = -1);
	const yuki_region* cutRegionBetween(const yuki_cursor& start, const yuki_cursor& end, int indent = -1);
	__inline const yuki_region* getRegion();
	__inline int getLineCount() const;
	/*
		��������Ż�ȡĳһ�У�������������ţ�����ڵ�ǰ�αꡣ
	*/
	const yuki_line_string* getLine(int offset = 0) const;
	__inline const yuki_cursor& getCursor() const;
	void setCursor(const yuki_cursor& cursor);
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
	yuki_region_stack_manager* m_regions;
	yuki_cursor m_cursor;					///< ��ǰ�����α꣬���ܵ�ǰ region ���ģ�ָ��Ķ��Ǿ���λ�á�
};