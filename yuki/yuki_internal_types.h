#pragma once

struct yuki_cursor
{
	int ln;	///< �кţ�0 ��ʼ
	int col;	///< �кţ�0 ��ʼ
	int ch;	///< �����ַ���ţ�0 ��ʼ
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
	yuki_region_type m_regionType;	///< ���������
	int m_sLn;	///< �������ʼ�кţ�0 ��ʼ
	int m_sCol;	///< �������ʼ�кţ�0 ��ʼ
	int m_eLn;	///< �������ֹ�кţ�0 ��ʼ��-1 ��ʾ����
	int m_eCol;	///< �������ֹ�кţ�0 ��ʼ��-1 ��ʾ����

	int m_sCh;	///< ������ʼ�������ַ����

	int m_indent;	///< ������������
};
