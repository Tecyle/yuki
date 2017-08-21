#include "stdafx.h"
#include "yuki.h"
#include "yuki_inline_markup.h"

bool YukiInlineMarkup::parse(YukiStruct* parent)
{
	m_fileLoader->saveCursor();

	if (!matchPrefix())
		goto match_failed;

	if (!searchInfix())
		goto match_failed;

	if (!searchSufix())
		goto match_failed;

	return true;
match_failed:
	m_fileLoader->restoreCursor();
	return false;
}

/*
	ǰ׺ʶ��ԭ��

	1. ������ǵĿ�ʼǰ׺��������ŵ��ַ����벻���ǿհ��ַ���
	4. ������ǵĿ�ʼǰ׺�ͽ�����׺֮ǰ�����ܽ�����һ��δת��ķ�б��(��
	   һ������������Ǿ����������ı��У���������з�б�ܵ�ת�庬�壬����
	   �������ı��Ľ�����׺֮ǰ�ǿ��Գ��ַ�б�ܵ�)��
	5. ���һ����ʼǰ׺֮ǰ������һ����ʼ����(ASCII �ַ����е� ``' " < ( [ {``
	   �����Ƿ� ASCII �ַ����е����Ƶķ���)����ô�����ʼǰ׺�ĺ���Ͳ��ܽ�����
	   ��Ӧ�Ĺرշ��� (ASCII �ַ����е� ' " > ) ] } �����Ƿ� ASCII �ַ����е���
	   �Ƶķ���)��Ŀǰ��yuki ��֧�� ASCII �Ŀ�ʼ���ź͹رշ��š�
	6. ���û�п��� simple-inline-markup ѡ��Ļ�����ô��������ǵĿ�ʼǰ׺����
	   ��һ���ı�����ʼ�������ǽ��������·��ź��棺
			
			* �հ��ַ�
			* ���б����ţ�- : / ' " < ( [ { �����������Ƶķ� ASCII �ַ����ı�����
*/
static bool matchPrefixRules(wchar_t prev, wchar_t next)
{
	const static wchar_t* rule6TokenSet = L"-:/'\"<([{������������";
	const static wchar_t* rule5StartSet = L"'\"<([{����������";
	const static wchar_t* rule5EndSet = L"'\">)]}����������";

	// ���� 6
	if (!yuki_simple_inline_markup())
	{
		if (prev != 0									// ����һ�е��ʼλ��
			&& prev != 1								// û�и���ת���ַ�����
			&& !isspace(prev)							// ���ǿհ��ַ�
			&& wcschr(rule6TokenSet, prev) == nullptr	// û�н���������Ŀ�ʼ���ź���
			)
			return false;
	}

	// ���� 4 �������̿��Ա�֤��ǰ׺���������ת��ᱻ����ת��
	if (prev == '\\')
		return false;

	// ���� 1
	if (isspace(next))
		return false;

	// ���� 5
	if (wcschr(rule5StartSet, prev) != nullptr
		&& wcschr(rule5EndSet, next) != nullptr)
		return false;

	return true;
}

/*
	��׺ʶ��ԭ��

	2. ������ǵĽ�����׺ǰ������ŵ��ַ����벻���ǿհ��ַ���
	3. ������ǵĿ�ʼǰ׺�ͽ�����׺֮�����������һ���ַ������仰˵����
	   ������������õ����ݲ����ǿյġ�
	4. ������ǵĿ�ʼǰ׺�ͽ�����׺֮ǰ�����ܽ�����һ��δת��ķ�б��(��
	   һ������������Ǿ����������ı��У���������з�б�ܵ�ת�庬�壬����
	   �������ı��Ľ�����׺֮ǰ�ǿ��Գ��ַ�б�ܵ�)��
	7. ���û�п��� simple-inline-markup ѡ��Ļ�����ô��������ǵĽ�����׺����
	   ��һ���ı���Ľ�β�����ߺ�����������·��ţ�
	
			* �հ��ַ�
			* ���б����ţ�- . , : ; ! ? \ / ' " ) ] } > �����������Ƶķ� ASCII �ַ����ı�����
*/
static bool matchSufixRules(wchar_t prev, wchar_t next, bool allowEscape)
{
	const static wchar_t* rule7Set = L"-.,:;!?\\/'\")]}>������������������������";

	// ������ 2
	if (isspace(next))
		return false;

	// ������ 4
	if (!allowEscape && next == '\\')
		return false;

	// ������ 7
	if (!yuki_simple_inline_markup())
	{
		if (!isspace(next) && wcschr(rule7Set, next) == nullptr)
			return false;
	}

	return true;
}

/*
	��׺ƥ�䣬��Ҫͬʱ����ǰ׺�ͺ�׺������
*/
inline static bool matchInfixRules(wchar_t prev, wchar_t next, bool allowEscape)
{
	return matchPrefixRules(prev, next) && matchSufixRules(prev, next, allowEscape);
}

bool YukiInlineMarkup::matchPrefix()
{
	wchar_t prevChar = m_fileLoader->peekPreviousCharInline();

	assert(m_prefix != nullptr);
	if (!m_fileLoader->match(m_prefix))
		return false;

	wchar_t curChar = m_fileLoader->getChar();

	return matchPrefixRules(prevChar, curChar);
}

bool YukiInlineMarkup::searchInfix()
{
	// ��������׺�Ļ���ֱ�ӳɹ�
	if (m_infix == nullptr)
		return true;

	return searchAndMatch(m_infix, &m_text1);
}

bool YukiInlineMarkup::searchSufix()
{
	return searchAndMatch(m_sufix, m_infix == nullptr ? &m_text1 : &m_text2);
}

bool YukiInlineMarkup::searchAndMatch(const wchar_t* matchStr, YukiString* readText)
{
	bool matchSucc = false;

	assert(matchStr != nullptr);

	wchar_t* textOffset = m_fileLoader->getCStr();
	int textLength = 0;
	wchar_t curChar = m_fileLoader->getChar();
	while (!outOfRegion())
	{
		if (!m_fileLoader->match(matchStr))
		{
			++textLength;
			m_fileLoader->moveToNextChar();
			curChar = m_fileLoader->getChar();
		}
		else
		{
			// ������ 3
			if (textLength == 0)
				return false;

			if (matchStr == m_infix)
				matchSucc = matchInfixRules(curChar, m_fileLoader->getChar(), m_allowEscapeNearMark);
			else
				matchSucc = matchSufixRules(curChar, m_fileLoader->getChar(), m_allowEscapeNearMark);

			if (matchSucc)
				break;

			// ƥ��ʧ�ܣ����������������
			textLength += wcslen(matchStr);
			m_fileLoader->moveToNextChar(wcslen(matchStr));
			curChar = m_fileLoader->getChar();
		}
	}

	if (!matchSucc)
		return false;

	readText->init(textOffset, textLength);
	return true;
}
