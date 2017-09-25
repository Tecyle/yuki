#pragma once
#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

/*
	yuki_line_string
	���ı�����ʾ yuki �ļ���һ��

	���ı�����������α���г��Ⱦ����ģ��г�����ָ�ַ����ȣ��������г��ȡ�

	���ı���ʶ���е�ǰ��������������������ʾ��ֻ�Ͽհ��ַ�����������ĩβ��
	ȥ������Ŀհ��ַ���������Ԥ�����ֿ�Ҳ���ĩβ�Ŀհ��ַ�ȥ������

	ע�⣬���ı����������ܺ������������һ�¡�

	��Ȼ���ı��ڲ��������α궼�Ǿ������꣬����ʹ�����ı����ⲿ�ӿڵ�ʱ��
	������αꡢ�±�֮���Ӧ��ʹ��������ꡣ
*/
class yuki_file_string;

class yuki_line_string
{
public:
	friend class yuki_file_string;
	friend class yuki_line_string_allocator;

	// ������ƶ�����һ��λ�ã�����Ĺ������Թ��
	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLineNumber() const { return m_lineHeadCursor.ln; }
	__inline int getLength() const { return m_length; }
	__inline int getContentLength() const;
	__inline const wchar_t* getContentPtr() const;
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }
	__inline int getIndent() const { return m_indent; }
	__inline bool isSeperator() const;

	__inline yuki_cursor beginCursor() const;
	__inline yuki_cursor endCursor() const;

	__inline int getOffset() const { return m_index; }

protected:
	// yuki_file_string ר�õĹ��캯����ͨ�����ƶ��� str ָ��������һ��
	void initByFileString(yuki_file_string* parent, int ln, wchar_t* & str);
	// YukiRegionString ר�ù��캯����ͨ����ȡһ�� YukiLineString ������һ��
	void initByRegion(const yuki_line_string* base, int startCol, int endCol);

	yuki_line_string(yuki_file_string* parent);
	yuki_line_string(const yuki_line_string* r);

	// �����к�����ȡ�����ַ���ţ������������ Tab �ַ�������� col ������к�
	int getChByCol(int col) const;

	__inline wchar_t* getCStr() const { return m_parent->m_buffer + m_index; }

private:
	yuki_file_string* m_parent;		///< ���� file_string���ַ�����Դͷ
	yuki_cursor m_lineHeadCursor;	///< ��������λ�õ��α꣬��������
	int m_length;					///< ��һ�еĳ��ȣ����������һ�����з����س����Żᱻȥ����
	int m_indent;					///< ��һ�е���������������������� YUKI_ERROR_INDENT��������һ���ǿ���
};
