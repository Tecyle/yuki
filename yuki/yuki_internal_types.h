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
	YukiRegion(const YukiRegion* region);

public:
	__inline void setSLn(int sLn) { m_sLn = sLn; }
	__inline int getSLn() const { return m_sLn; }
	__inline void setSCol(int sCol) { m_sCol = sCol; }
	__inline int getSCol() const { return m_sCol; }
	__inline void setELn(int eLn) { m_eLn = eLn; }
	__inline int getELn() const { return m_eLn; }
	__inline void setECol(int eCol) { m_eCol = eCol; }
	__inline int getECol() const { return m_eCol; }
	__inline yuki_region_type getRegionType() const { return m_regionType; }
	__inline void setSCh(int sCh) { m_sCh = sCh; }
	__inline int getSCh() const { return m_sCh; }
	__inline void setIndent(int indent) { m_indent = indent; }
	__inline int getIndent() { return m_indent; }

	virtual bool outOfRegion();

protected:
	yuki_region_type m_regionType;	///< 区域的类型
	int m_sLn;	///< 区域的起始行号，0 开始
	int m_sCol;	///< 区域的起始列号，0 开始
	int m_eLn;	///< 区域的终止行号，0 开始，-1 表示无穷
	int m_eCol;	///< 区域的终止列号，0 开始，-1 表示无穷

	int m_sCh;	///< 区间起始的行内字符序号

	int m_indent;	///< 这个区域的缩进
};
