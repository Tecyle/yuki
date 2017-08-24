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
	
protected:
	yuki_region_type m_regionType;	///< ���������
	int m_sLn;	///< �������ʼ�кţ�0 ��ʼ
	int m_sCol;	///< �������ʼ�кţ�0 ��ʼ
	int m_eLn;	///< �������ֹ�кţ�0 ��ʼ��-1 ��ʾ����
	int m_eCol;	///< �������ֹ�кţ�0 ��ʼ��-1 ��ʾ����

	int m_sCh;	///< ������ʼ�������ַ����
};