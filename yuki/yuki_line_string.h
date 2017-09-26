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
	friend class yuki_line_string_manager;

	// ������ƶ�����һ��λ�ã�����Ĺ������Թ��
	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLineNumber() const { return m_lineHeadCursor.ln; }
	__inline int getLength() const { return m_length; }
	__inline int getContentLength() const { return m_contentLength; }
	__inline const wchar_t* getContentPtr() const;
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }
	__inline int getIndent() const { return m_indent; }
	__inline bool isSeperator() const;

	__inline yuki_cursor beginCursor() const { return m_lineHeadCursor; }
	__inline yuki_cursor endCursor() const { return m_contentTailCursor; }
	__inline yuki_cursor contentBeginCursor() const { return m_contentHeadCursor; }
	__inline yuki_cursor contentEndCursor() const { return m_contentTailCursor; }

	__inline int getOffset() const { return m_lineHeadCursor.offset; }

protected:
	yuki_line_string(yuki_file_string* parent);
	yuki_line_string(const yuki_line_string* r);

	// YukiRegionString ר�ù��캯����ͨ����ȡһ�� YukiLineString ������һ��
	void initByRegion(const yuki_line_string* base, int startCol, int endCol);

	// �����к�����ȡ�����ַ���ţ������������ Tab �ַ�������� col ������к�
	int getChByCol(int col) const;

	__inline wchar_t* getCStr() const { return m_parent->m_buffer + getOffset(); }

private:
	yuki_file_string* m_parent;			///< ���� file_string���ַ�����Դͷ
	yuki_cursor m_lineHeadCursor;		///< ��������λ�õ��α꣬��������
	yuki_cursor m_contentHeadCursor;	///< ���ݲ��ֿ�ʼ���α꣬��������
	yuki_cursor m_contentTailCursor;	///< ���ݲ��ֽ������α꣬��������
	int m_length;						///< ��һ�еĳ��ȣ����������һ�����з����س����Żᱻȥ����
	int m_contentLength;				///< ���岿�ֵĳ��ȣ�ȥ����β�ո�ĳ���
	int m_indent;						///< ��һ�е���������������������� YUKI_ERROR_INDENT��������һ���ǿ���
};
