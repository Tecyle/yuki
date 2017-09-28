#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_markup.h"

bool YukiInlineMarkup::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	yuki_cursor startCursor;
	yuki_cursor endCursor;
	bool succ = false;

	m_textRegion1 = nullptr;
	m_textRegion2 = nullptr;

	// ƥ��ǰ׺
	if (!matchPrefix())
		goto match_finished;

	startCursor = reader->getCursor();
	// ���������׺��ƥ����׺
	if (m_infix != nullptr)
	{
		if (!matchInfix(endCursor))
			goto match_finished;

		m_textRegion1 = reader->cutRegionBetween(startCursor, endCursor);
		startCursor = reader->getCursor();
	}

	// ƥ���׺
	if (!matchSuffix(endCursor))
		goto match_finished;

	if (m_infix == nullptr)
		m_textRegion1 = reader->cutRegionBetween(startCursor, endCursor);
	else
		m_textRegion2 = reader->cutRegionBetween(startCursor, endCursor);

	// ��¼ƥ��ɹ�֮���λ�ã���ֹ�ظ�����
	m_endCursor = reader->getCursor();
	succ = true;

match_finished:
	reader->setCursor(oldCursor);
	return succ;
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
const static wchar_t* rule6TokenSet = L"-:/'\"<([{������������";
const static wchar_t* rule5StartSet = L"'\"<([{����������";
const static wchar_t* rule5EndSet = L"'\">)]}����������";
const static wchar_t* rule7Set = L"-.,:;!?\\/'\")]}>������������������������";
static bool matchPrefixRules(wchar_t prev, wchar_t next)
{

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

bool YukiInlineMarkup::matchPrefix()
{
	yuki_file_reader* reader = getFileReader();
	wchar_t prevChar = reader->peekPreviousChar();

	assert(m_prefix != nullptr);
	if (!reader->matchStr(m_prefix))
		return false;

	wchar_t curChar = reader->getChar();

	return matchPrefixRules(prevChar, curChar);
}

bool YukiInlineMarkup::matchInfix(yuki_cursor& cursor)
{
	assert(m_infix != nullptr);
	return searchAndMatch(m_infix, cursor);
}

bool YukiInlineMarkup::matchSuffix(yuki_cursor& cursor)
{
	return searchAndMatch(m_suffix, cursor);
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
	// ���� 6
	if (!yuki_simple_inline_markup())
	{
		if (prev != 0									// ����һ�е��ʼλ��
			&& !isspace(prev)							// ���ǿհ��ַ�
			&& wcschr(rule6TokenSet, prev) == nullptr	// û�н���������Ŀ�ʼ���ź���
			)
			return false;
	}

	return matchPrefixRules(prev, next) && matchSufixRules(prev, next, allowEscape);
}

bool YukiInlineMarkup::searchAndMatch(const wchar_t* pattern, yuki_cursor& contentCursor)
{
	yuki_file_reader* reader = getFileReader();
	bool matchSucc = false;

	assert(pattern != nullptr);

	int textLength = 0;
	wchar_t lastChar = 0;
	yuki_cursor lastCursor;
	for(;reader->moveToNextChar();)
	{
		lastCursor = reader->getCursor();
		if (!reader->matchStr(pattern))
		{
			textLength++;
			lastChar = reader->getChar();
			continue;
		}
		// ����ƥ���ģʽ
		// ������ 3
		if (textLength == 0)
			return false;

		if (pattern == m_infix)
			matchSucc = matchInfixRules(lastChar, reader->getChar(), m_allowEscapeNearMark);
		else
			matchSucc = matchSufixRules(lastChar, reader->getChar(), m_allowEscapeNearMark);

		if (matchSucc)
			break;

		// ƥ��ʧ�ܣ����������������
		textLength += wcslen(pattern);
		lastChar = reader->getChar();
	}

	if (!matchSucc)
		return false;

	contentCursor = lastCursor;
	return true;
}
