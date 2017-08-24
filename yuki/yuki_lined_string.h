#pragma once

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

protected:
	// YukiLinedString ר�õĹ��캯����ͨ�����ƶ��� str ָ��������һ��
	YukiStringLine(YukiLinedString* parent, int ln, wchar_t* & str);

private:
	YukiLinedString* m_parent;
	int m_ln;	///< ��һ�����ڵ��к�
	int m_index;	///< ��һ���������ַ������±�λ��
	int m_length;	///< ��һ�еĳ��ȣ��������һ�����з����س����Żᱻȥ����
};

class YukiRegionString
{
public:
	YukiRegionString(YukiLinedString* parent);
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

protected:
	bool buildFromBuffer();

private:
	wchar_t* m_buffer;	///< ԭʼ�ı���Ϣ
	int m_bufferLength;	///< ԭʼ�ı�����

	YukiStringLine* m_lines;	///< �洢���е�����Ϣ
	int m_lineCount;	///< һ���ж�����
};