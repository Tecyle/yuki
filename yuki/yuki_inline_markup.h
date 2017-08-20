#pragma once

/*
	���е�������Ƕ������ʽ��

	[ǰ׺] �ı�1 [��׺] �ı�2 [��׺]

	���У���׺ �� �ı�2 �ǿ���ʡ�Եġ�
*/
class YukiInlineMarkup : public YukiStruct
{
public:
	virtual bool parse(YukiStruct* parent);

protected:
	virtual bool matchPrefix();
	virtual bool searchInfix();
	virtual bool searchSufix();

protected:
	const wchar_t* m_prefix;
	const wchar_t* m_infix;
	const wchar_t* m_sufix;

	YukiString m_text1;
	YukiString m_text2;

	bool m_allowEscapeNearMark;		///< ����ǰ׺�����Լ���׺ǰ���Ƿ���Խ���һ��ת���ַ�
};