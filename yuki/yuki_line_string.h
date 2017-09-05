#pragma once
#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

class YukiFileString;

class YukiLineString
{
public:
	friend class YukiFileString;
	friend class YukiLinedRegionString;
	friend class YukiBlockRegionString;

	// ������ƶ�����һ��λ�ã�����Ĺ������Թ��
	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLn() const { return m_ln; }
	__inline int getLength() const { return m_length; }
	__inline int getContentLength() const;
	__inline const wchar_t* getContentPtr() const;
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }
	__inline int getIndent() const { return m_indent; }
	__inline bool isSeperator() const;

	__inline yuki_cursor beginCursor() const;
	__inline yuki_cursor endCursor() const;

protected:
	// YukiFileString ר�õĹ��캯����ͨ�����ƶ��� str ָ��������һ��
	YukiLineString(YukiFileString* parent, int ln, wchar_t* & str);
	// YukiRegionString ר�ù��캯����ͨ����ȡһ�� YukiLineString ������һ��
	YukiLineString(const YukiLineString* base, int startCol, int endCol);

	YukiLineString(const YukiLineString* r);

	// �����к�����ȡ�����ַ���ţ������������ Tab �ַ�������� col ������к�
	int getChByCol(int col) const;

	__inline wchar_t* getCStr() const { return m_parent->m_buffer + m_index; }

private:
	YukiFileString* m_parent;
	int m_ln;	///< ��һ�����ڵ��кţ�ȫ��
	int m_col;	///< ��һ�еĿ�ʼ�кţ�ȫ��
	int m_ch;	///< ��һ�е������ַ���ţ�ȫ��
	int m_index;	///< ��һ���������ַ������±�λ�ã�ȫ��
	int m_length;	///< ��һ�еĳ��ȣ����������һ�����з����س����Żᱻȥ����
	int m_indent;	///< ��һ�е���������������������� YUKI_ERROR_INDENT��������һ���ǿ���
};
