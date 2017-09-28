#pragma once

class yuki_file_string;

class yuki_region_string
{
public:
	yuki_region_string(yuki_file_string* parent, const yuki_region* region);
	~yuki_region_string();

public:
	/*
		获取 cursor 位置对应的 line，绝对游标。
		如果获取失败，返回 nullptr。
	*/
	const yuki_line_string* getLineStringAtCursor(const yuki_cursor& cursor) const;

	/*
		这里的 ln 是相对行数。
	*/
	const yuki_line_string* getLine(int ln) const { return m_lines[ln]; }
	int getLineCount() const { return m_lineCount; }

	__inline const yuki_region* getRegion() const { return m_region; }

protected:
	/*
		以下两个构造函数仅可在 file string 中调用，为了是避免不必要的构造
		line string 数组。所以，如果是通过以下两个构造函数构造的话，将不会
		去释放 line string 数组。
	*/
	yuki_region_string(yuki_file_string* parent, const yuki_region* region,
		const yuki_line_string** lines, int lineCount)
		: m_parent(parent)
		, m_region(region)
		, m_lines(lines)
		, m_lineCount(lineCount)
		, m_needRelease(false)
	{}
	yuki_region_string(const yuki_region_string* r);

	void buildLineStringArray();

protected:
	yuki_file_string* m_parent;
	const yuki_region* m_region;
	const yuki_line_string** m_lines;
	int m_lineCount;
	bool m_needRelease;

	friend class yuki_file_string;
};
