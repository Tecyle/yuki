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
	// YukiLinedString 专用的构造函数，通过可移动的 str 指针来构造一行
	YukiStringLine(YukiLinedString* parent, int ln, wchar_t* & str);

private:
	YukiLinedString* m_parent;
	int m_ln;	///< 这一行所在的行号
	int m_index;	///< 这一行在整个字符串的下标位置
	int m_length;	///< 这一行的长度，包括最后一个换行符（回车符号会被去掉）
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
	// -1 代表最后一行，以此类推
	__inline const YukiStringLine* getLine(int index);

protected:
	bool buildFromBuffer();

private:
	wchar_t* m_buffer;	///< 原始文本信息
	int m_bufferLength;	///< 原始文本长度

	YukiStringLine* m_lines;	///< 存储所有的行信息
	int m_lineCount;	///< 一共有多少行
};