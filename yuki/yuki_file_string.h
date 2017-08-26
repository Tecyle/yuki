#pragma once

class YukiLineString;

class YukiFileString
{
public:
	friend class YukiLineString;
	YukiFileString();
	~YukiFileString();

	bool loadFromFile(const wchar_t* fileName);

	// Properties
public:
	// -1 代表最后一行，以此类推
	__inline const YukiLineString* getLine(int index);
	__inline const int getLineCount() const { return m_lineCount; }
	const YukiRegionString* getRegionString(const YukiRegion* region);

protected:
	bool buildFromBuffer();

private:
	wchar_t* m_buffer;	///< 原始文本信息
	int m_bufferLength;	///< 原始文本长度

	YukiLineString* m_lines;	///< 存储所有的行信息
	int m_lineCount;	///< 一共有多少行
};