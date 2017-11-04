#pragma once

/*
	���е�������Ƕ������ʽ��

	[ǰ׺] �ı�1 [��׺] �ı�2 [��׺]

	���У���׺ �� �ı�2 �ǿ���ʡ�Եġ�

	ʹ�÷�����
	�̳б���󣬰�������ǰ׺��׺�ͺ�׺���Լ�����Ƿ���Ժ��ת���
	Ȼ���� parse �����е��� match�����Ի�ȡ
	m_textRegion1 : �ı�1 ������
	m_textRegion2 : �ı�2 ������
	m_endCursor : ����λ�õĹ��

	���಻����ʹ��
*/
class yuki_inline_markup : public yuki_structure_parser
{
public:
	yuki_inline_markup(yuki_session* globalData) : yuki_structure_parser(globalData) {}

public:
	virtual bool match() override;

protected:
	virtual bool matchPrefix();
	virtual bool matchInfix(yuki_cursor& cursor);
	virtual bool matchSuffix(yuki_cursor& cursor);

	virtual bool searchAndMatch(const wchar_t* pattern, yuki_cursor& contentCursor);

protected:
	const wchar_t* m_prefix;
	const wchar_t* m_infix;
	const wchar_t* m_suffix;

	const yuki_region* m_textRegion1;
	const yuki_region* m_textRegion2;

	yuki_cursor m_endCursor;

	bool m_allowEscapeNearMark;		///< ����ǰ׺�����Լ���׺ǰ���Ƿ���Խ���һ��ת���ַ�
};