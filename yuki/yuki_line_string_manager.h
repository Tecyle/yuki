#pragma once

class yuki_line_string_allocator;
class yuki_line_string;
class yuki_file_string;

/*
	yuki_line_string_manager
	负责 yuki_line_string 的生成的释放，并实现附加操作。

	这个类是单例工作模式。
*/
class yuki_line_string_manager
{
public:
	static __inline yuki_line_string_manager* getInstance() { return &m_instance; }

protected:
	yuki_line_string_manager();
	yuki_line_string_manager(const yuki_line_string_manager&) {}
	yuki_line_string_manager& operator=(const yuki_line_string_manager&) {}
	~yuki_line_string_manager();

public:
	__inline const yuki_line_string* allocLineStringForFileString(yuki_file_string* fileString,
		int ln, wchar_t* & str);

	__inline const yuki_region* cloneRegion(const yuki_region* region);

	__inline void freeRegion(const yuki_region* region);

	/*
	指定起终点以及切割方式，从 originalRegion 中切割一个采用绝对坐标的
	子区域并返回。
	*/
	const yuki_region* allocFromSubRegion(yuki_file_string* buffer, const yuki_region* originalRegion,
		const yuki_cursor& startPos, const yuki_cursor& endPos, int indent,
		yuki_region_type type = Yuki_linedRegion);

protected:
	yuki_region_allocator* m_allocator;			///< region 分配器
	static yuki_region_manager m_instance;		///< 单例实例
};