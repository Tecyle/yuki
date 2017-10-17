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

	/*
		�ӵ�ǰ region �вü�һ���� ��ǰ�α� �� cursor ��һ������������
		���������ָ��Ϊ indent��

		���ָ�� indent Ϊ -1���� indent ���� ��ǰ���� �� indent��
	*/
	__inline const yuki_region* cutRegionFromCursorTo(const yuki_cursor& cursor, int indent = -1);

	/*
		�ӵ�ǰ region �вü�һ���� ��ǰ�α� �� ��ǰ�����ĩβ�α� ��һ��
		����������

		���ָ�� indent Ϊ -1���� indent ���� ��ǰ���� �� indent��
	*/
	__inline const yuki_region* cutRegionFromCursorToEnd(int indent = -1);
	
	/*
		�ӵ�ǰ region �вü�һ���� cursor �� ��ǰ�α� ��һ������������
		���������ָ��Ϊ indent��

		���ָ�� indent Ϊ -1���� indent ���� ��ǰ���� �� indent��
	*/
	const yuki_region* cutRegionToCursorFrom(const yuki_cursor& cursor, int indent = -1);

	/*
		�ӵ�ǰ region �вü�һ���� start �� end ��һ������������
		���������ָ��Ϊ indent��

		���ָ�� indent Ϊ -1���� indent ���� ��ǰ���� �� indent��
	*/
	const yuki_region* cutRegionBetween(const yuki_cursor& start, const yuki_cursor& end, int indent = -1);
	__inline const yuki_region* getRegion() const;
	__inline int getLineCount() const;
	/*
		��������Ż�ȡĳһ�У�������������ţ�����ڵ�ǰ�αꡣ
	*/
	const yuki_line_string* getLine(int offset = 0) const;
	__inline const yuki_cursor& getCursor() const;
	void setCursor(const yuki_cursor& cursor);

	/*
		�ӵ�ǰ�α�λ�ÿ�ʼ��ƥ��һ���ַ��� str��
		ƥ��ɹ����� true�����ƶ��α굽ƥ����ַ������档
		���ƥ��ʧ�ܣ����� false�����Ҳ��ƶ��αꡣ

		����ƥ�䲻�ܿ��С�
		ƥ����ַ����в����� \t �ַ���
	*/
	bool matchStr(const wchar_t* str);
	bool matchChar(wchar_t ch);

	/*
		ƥ�䲢������ǰλ�ÿ�ʼ�����������հ��ַ���
		ע�⣬Tab �ַ��ỻ��ɵ�ͬ�����Ŀո�

		���������Ŀո�����
		�ò����������ڽ��С�
	*/
	int skipSpaces();
	bool cursorAtLineEnd() const;
	bool moveToNextLine();
	/* 
		����ĩβ�������ո񣩲������� count �� ch �ַ���
		���ƥ��ɹ�������ָ��ƥ���ַ������Ĺ�꣬
		���ƥ��ʧ�ܣ�������β��ꡣ
		ƥ�䲻���ƶ���ǰ��ꡣ

		Ŀǰ���ں�׺ƥ�䡣

		���ƥ��ʧ�ܣ��᷵��һ����Ч�αꡣ
	*/
	yuki_cursor findSuffixChar(wchar_t ch, int count = 0);

	// �ú������������������
	bool moveToNextChar(bool crossLines = true);
	wchar_t getChar(int offset = 0);
	wchar_t peekPreviousChar();

	/*
		�ӵ�ǰ�α�λ�ÿ�ʼ��������� str �Ƿ���֡�
		crossLine ָ���Ƿ����������
	*/
	bool searchStr(const wchar_t* str, bool crossLine = false);
	bool matchBlankLine();

	/*
		��ǰ�α��Ƿ񵽴���߳�����Χĩβ��
	*/
	bool outOfRegion() const;

protected:
	yuki_region_stack_manager* m_regions;
	yuki_cursor m_cursor;					///< ��ǰ�����α꣬���ܵ�ǰ region ���ģ�ָ��Ķ��Ǿ���λ�á�
};