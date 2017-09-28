#pragma once

class yuki_region_stack_manager;

/*
	yuki_file_reader
	yuki 文件读取类

	注意，所使用的 cursor 都是绝对位置游标，尽量不要去自己构造 cursor。
*/
class yuki_file_reader
{
public:
	yuki_file_reader();
	~yuki_file_reader();

public:
	bool openFile(const wchar_t* fileName);

	/*
		将 region 选择为当前生效的 region，并压入 region 栈。
	*/
	__inline void pushRegion(const yuki_region* region);
	/*
		丢弃当前 region，并恢复之前的 region。
	*/
	__inline void popRegion();

	/*
		跳过空行，返回跳过的行数。
		如果没跳过任何空行，返回 0。
	*/
	int skipBlankLines();
	const yuki_region* cutRegionFromCursorTo(const yuki_cursor& cursor, int indent = -1);
	const yuki_region* cutRegionFromCursorToEnd();
	// 如果提供了 indent，则使用这个值作为 indent，如果是 -1，则自动计算 indent
	const yuki_region* cutRegionToCursorFrom(const yuki_cursor& cursor, int indent = -1);
	const yuki_region* cutRegionBetween(const yuki_cursor& start, const yuki_cursor& end, int indent = -1);
	__inline const yuki_region* getRegion();
	__inline int getLineCount() const;
	/*
		根据行序号获取某一行，行序号是相对序号，相对于当前游标。
	*/
	const yuki_line_string* getLine(int offset = 0) const;
	__inline const yuki_cursor& getCursor() const;
	void setCursor(const yuki_cursor& cursor);
	bool matchStr(const wchar_t* str);
	bool matchChar(wchar_t ch);
	int skipSpaces();
	bool cursorAtLineEnd() const;
	bool moveToNextLine();
	/* 
		从行末尾（跳过空格）查找连续 count 个 ch 字符，
		如果匹配成功，返回指向匹配字符串起点的光标，
		如果匹配失败，返回行尾光标。
		匹配不会移动当前光标。
	*/
	yuki_cursor findSuffixChar(wchar_t ch, int count = 0);

	// 该函数会跳过区域的缩进
	bool moveToNextChar(bool crossLines = true);
	wchar_t getChar(int offset = 0);
	wchar_t peekPreviousChar();

	bool searchStr(const wchar_t* str, bool crossLine = false);
	bool matchBlankLine();

protected:
	yuki_region_stack_manager* m_regions;
	yuki_cursor m_cursor;					///< 当前解析游标，不管当前 region 是哪，指向的都是绝对位置。
};