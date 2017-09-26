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
	/*
		从全局的 yuki_file_string 全局文件来构造一个 line string。
		ln 代表当前的行号，
		str 代表当前的字符指针，会被该函数修改到下一行行首。
	*/
	const yuki_line_string* allocLineStringForFileString(yuki_file_string* fileString,
		int ln, wchar_t* & str);

	__inline const yuki_line_string* cloneLineString(const yuki_line_string* str);

	__inline void freeLineString(const yuki_line_string* str);

private:
	__inline yuki_line_string* allocLineString(yuki_file_string* fileString);

protected:
	yuki_line_string_allocator* m_allocator;		///< region 分配器
	static yuki_line_string_manager m_instance;		///< 单例实例
};

#define yukiLineStringManager() (yuki_line_string_manager::getInstance())