#pragma once

/*
	定义一个 yuki 文档的解析结构，需要定义以下内容：


*/

class yuki_structure
{
public:
	yuki_structure(yuki_session* session);
	virtual ~yuki_structure();

public:
	// 解析
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) = 0;
	virtual bool match() = 0;

	__inline const wchar_t* getName() const { return m_name; }

protected:
	__inline yuki_structure* getParser(const wchar_t* name);
	__inline yuki_file_reader* getFileReader();

protected:
	const wchar_t* m_name;
	yuki_struct_type m_type;
	yuki_session* m_session;
};
