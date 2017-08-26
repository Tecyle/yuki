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
	// YukiLinedString 专用的构造函数，通过可移动的 str 指针来构造一行
	YukiStringLine(YukiLinedString* parent, int ln, wchar_t* & str);

private:
	YukiLinedString* m_parent;
	int m_ln;	///< 这一行所在的行号
	int m_index;	///< 这一行在整个字符串的下标位置
	int m_length;	///< 这一行的长度，不包括最后一个换行符（回车符号会被去掉）
	int m_indent;	///< 这一行的缩进级别，如果缩进级别是 YUKI_ERROR_INDENT，表明这一行是空行
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
	// -1 代表最后一行，以此类推
	__inline const YukiStringLine* getLine(int index);
	__inline const int getLineCount() const { return m_lineCount; }
	const YukiRegionString* getRegionString(const YukiRegion* region);

protected:
	bool buildFromBuffer();

private:
	wchar_t* m_buffer;	///< 原始文本信息
	int m_bufferLength;	///< 原始文本长度

	YukiStringLine* m_lines;	///< 存储所有的行信息
	int m_lineCount;	///< 一共有多少行
};