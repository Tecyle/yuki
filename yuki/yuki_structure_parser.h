#pragma once

/*
	定义一个 yuki 文档的解析器
*/

class yuki_structure_parser
{
public:
	yuki_structure_parser(yuki_session* session);
	virtual ~yuki_structure_parser();

public:
	/*
		解析函数，会去解析该结构，如果解析成功，返回 true，否则返回 false。
		如果解析成功，游标会移动到这个结构的下一个位置；
		如果解析失败，游标将不会移动。

		parentNode
			代表了当前解析结构的父节点，当前解析得到的节点将会追加在这个节点
			下面的最后一个子节点的后面。

		region
			解析的区域，限定了解析的范围在 region 所指向的范围内进行。
	*/
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) = 0;
	
	/*
		匹配函数，目的在于试探当前位置是否能成功解析该结构，如果可以解析，则
		返回 true，否则返回 false。

		如果这个函数返回 false，那么调用 parse 返回的也应该是 false。

		不管解析成功与否，游标都不会改变原来的位置。

		在匹配的时候，仅在当前区域进行搜索。
	*/
	virtual bool match() = 0;

public:
	__inline const wchar_t* getName() const { return m_name; }
	__inline yuki_structure_type getStructureType() const { return m_type; }

protected:
	/*
		辅助函数，根据结构的名称来获取对应的解析器。
	*/
	__inline yuki_structure_parser* getParser(const wchar_t* name);
	
	/*
		辅助函数，获得当前会话的 reader。
	*/
	__inline yuki_file_reader* getFileReader();

protected:
	const wchar_t* m_name;				///< 当前结构解析器名称
	yuki_structure_type m_type;			///< 当前结构解析器的类型
	yuki_session* m_session;			///< 保存当前会话
};
