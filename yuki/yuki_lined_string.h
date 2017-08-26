#pragma once

#define YUKI_ERROR_INDENT -1
#define YUKI_AUTO_CALC_INDENT -2

class YukiLinedString;

class YukiStringLine
{
public:
	friend class YukiLinedString;

	// properties
public:
	__inline int getLn() const { return m_ln; }
	__inline int getLength() const { return m_length; }
	__inline wchar_t getCharAtIndex(int ch) const;
	__inline bool isBlankLine() const { return m_indent == YUKI_ERROR_INDENT; }

protected:
	// YukiLinedString ר�õĹ��캯����ͨ�����ƶ��� str ָ��������һ��
	YukiStringLine(YukiLinedString* parent, int ln, wchar_t* & str);

private:
	YukiLinedString* m_parent;
	int m_ln;	///< ��һ�����ڵ��к�
	int m_index;	///< ��һ���������ַ������±�λ��
	int m_length;	///< ��һ�еĳ��ȣ����������һ�����з����س����Żᱻȥ����
	int m_indent;	///< ��һ�е���������������������� YUKI_ERROR_INDENT��������һ���ǿ���
};

class YukiRegionString
{
public:
	YukiRegionString(YukiLinedString* parent) { m_parent = parent; }

	YukiRegionString* getRegionString(const YukiRegion* region);

protected:
	YukiLinedString* m_parent;
	YukiRegion* m_region;
};

class YukiLinedString
{
public:
	friend class YukiStringLine;
	YukiLinedString();
	~YukiLinedString();

	bool loadFromFile(const wchar_t* fileName);

	// Properties
public:
	// -1 �������һ�У��Դ�����
	__inline const YukiStringLine* getLine(int index);
	__inline const int getLineCount() const { return m_lineCount; }
	const YukiRegionString* getRegionString(const YukiRegion* region);

protected:
	bool buildFromBuffer();

private:
	wchar_t* m_buffer;	///< ԭʼ�ı���Ϣ
	int m_bufferLength;	///< ԭʼ�ı�����

	YukiStringLine* m_lines;	///< �洢���е�����Ϣ
	int m_lineCount;	///< һ���ж�����
};