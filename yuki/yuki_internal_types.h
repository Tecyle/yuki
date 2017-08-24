#pragma once

struct yuki_cursor
{
	int ln;	///< 行号，0 开始
	int col;	///< 列号，0 开始
	int ch;	///< 行内字符序号，0 开始
};

enum yuki_region_type
{
	Yuki_linedRegion,
	Yuki_blockRegion
};

class YukiRegion
{
public:
	
protected:
	yuki_region_type m_regionType;	///< 区域的类型
	int m_sLn;	///< 区域的起始行号，0 开始
	int m_sCol;	///< 区域的起始列号，0 开始
	int m_eLn;	///< 区域的终止行号，0 开始，-1 表示无穷
	int m_eCol;	///< 区域的终止列号，0 开始，-1 表示无穷

	int m_sCh;	///< 区间起始的行内字符序号
};