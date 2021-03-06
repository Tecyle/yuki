#pragma once

class yuki_line_string;

/*
	YukiFileString
	
	表示一个 yuki 文件的所有字符信息，包括一系列便携访问函数。
	所有的 region 和 line 都是从这个对象中引申出来的。
*/
class yuki_file_string
{
public:
	friend class yuki_line_string;
	yuki_file_string();
	~yuki_file_string();

	/*
		从一个文件中加载 yuki 内容
		TODO 目前只支持 UTF-8，以后扩展支持多种编码
	*/
	bool loadFromFile(const wchar_t* fileName);

	// Properties
public:
	/*
		返回一行，-1 代表最后一行
	*/
	__inline const yuki_line_string* getLine(int index);

	/*
		返回行的总数
	*/
	__inline const int getLineCount() const { return m_lineCount; }

	/*
		给定一个 Region，返回对应的 RegionString。
		如果传入 nullptr，则返回一个代表整个文件的 region string。
	*/
	const yuki_region_string* allocRegionString(const yuki_region* region);

	/*
		计算相对于 offset 的游标 target 的绝对游标，结果保存在
		target 中。
	*/
	void addCursorOffsetToCursor(yuki_cursor& target, const yuki_cursor& offset);

	__inline const wchar_t* getBuffer() const { return m_buffer; }

protected:
	bool buildFromBuffer();
	void buildWholeFileRegionString();

private:
	wchar_t* m_buffer;					///< 原始文本信息
	int m_bufferLength;					///< 原始文本长度

	const yuki_line_string** m_lines;	///< 存储所有的行信息
	int m_lineCount;					///< 一共有多少行

	yuki_region_string* m_wholeFileRegionString;
};