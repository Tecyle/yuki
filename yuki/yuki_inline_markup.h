#pragma once

/*
	���е�������Ƕ������ʽ��

	[ǰ׺] �ı�1 [��׺] �ı�2 [��׺]

	���У���׺ �� �ı�2 �ǿ���ʡ�Եġ�
*/
class YukiInlineMarkup : public YukiStruct
{
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
	const wchar_t* m_sufix;

	const YukiRegion* m_textRegion1;
	const YukiRegion* m_textRegion2;

	yuki_cursor m_endCursor;

	bool m_allowEscapeNearMark;		///< ����ǰ׺�����Լ���׺ǰ���Ƿ���Խ���һ��ת���ַ�
};