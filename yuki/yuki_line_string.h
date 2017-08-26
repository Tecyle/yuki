#pragma once
#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

class YukiFileString;

class YukiLineString
{
public:
	friend class YukiFileString;

	bool moveCursorToNext(yuki_cursor& cursor);

	// properties
public:
	__inline int getLn() const { return m_ln; }
	__inline int getLength() const { return m_length; }
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }

protected:
	// YukiFileString ר�õĹ��캯����ͨ�����ƶ��� str ָ��������һ��
	YukiLineString(YukiFileString* parent, int ln, wchar_t* & str);
	// YukiRegionString ר�ù��캯����ͨ����ȡһ�� YukiLineString ������һ��
	YukiLineString(YukiLineString* base, int startCol, int endCol);

	// �����к�����ȡ�����ַ���ţ������������ Tab �ַ�
	int getChByCol(int col);

private:
	YukiFileString* m_parent;
	int m_ln;	///< ��һ�����ڵ��к�
	int m_index;	///< ��һ���������ַ������±�λ��
	int m_length;	///< ��һ�еĳ��ȣ����������һ�����з����س����Żᱻȥ����
	int m_indent;	///< ��һ�е���������������������� YUKI_ERROR_INDENT��������һ���ǿ���
};
