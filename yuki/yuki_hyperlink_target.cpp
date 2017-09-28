#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_hyperlink_target.h"

bool YukiHyperlinkTarget::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);

	if (!matchNoBackward())
	{
		reader->setCursor(oldCursor);
		return false;
	}

	YukiHyperlinkTargetNode* node = new YukiHyperlinkTargetNode;

	if (!getParser(L"link_uri")->parse(node, reader->cutRegionFromCursorToEnd()))
	{
		node->setTargetType(HyperlinkTarget_internal);
	}

	return true;
}

bool YukiHyperlinkTarget::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

/*
	ʶ������Ŀ����߼���

	1. ``_`` ��ͷ
	2. �»��ߺ������û�з����ŵĻ�
		1. �»��ߺ��治���ǿհ��ַ�
		2. ����ð�Ž���
		3. ð��ǰ�治���ǿհ��ַ�
		4. �»��ߺ�ð��֮��������һ���ַ�
	3. �»��ߺ�������Ƿ����ŵĻ�
		1. ���� `: ����
		2. ������ǰ�����пո�
		3. �������ڲ�������һ���ַ�
	4. ð�ź�������пո�
	5. ��ʶ���ʱ�����ĸ���ֵĵط���ͳһ���滻�ɵ�һ�ļ���
*/
bool YukiHyperlinkTarget::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();

	// ��ʶ����������Ŀ��
	if (reader->matchStr(L"__:"))
	{
		if (!reader->cursorAtLineEnd() && reader->skipSpaces() == 0)
			return false;

		m_anonymousName = true;
		return true;
	}

	// �����ģʽ������������
	m_anonymousName = false;
	if (!reader->matchChar('_'))
		return false;
	// ƥ����������
	if (reader->matchChar('`'))
	{		
		if (!readTargetName('`'))
			return false;

		if (!reader->matchChar(':'))
			return false;
	}
	else
	{
		if (!readTargetName(':'))
			return false;
	}
	// ƥ������ո�
	if (!reader->cursorAtLineEnd() && reader->skipSpaces() == 0)
		return false;

	return true;
}

bool YukiHyperlinkTarget::readTargetName(wchar_t endChar)
{
	yuki_file_reader* reader = getFileReader();
	wchar_t ch = reader->getChar();
	bool lastCharIsWordChar = true;
	bool lastCharIsSpace = false;
	m_matchedName = L"";

	if (isspace(ch) || ch == endChar)
		return false;

	do
	{
		ch = reader->getChar();
		if (ch == 0)
			return false;
		// �ж��Ƿ�������������
		if (ch == endChar)
		{
			// ������֮ǰ�����з�ת��Ŀո�
			if (lastCharIsSpace)
				return false;

			if (m_matchedName.size() > 0)
			{
				// �Ƴ�ĩβ�� - ����
				if (m_matchedName.back() == '-')
					m_matchedName.pop_back();
			}
			reader->moveToNextChar(false);
			return true;
		}
		// ����ת���ַ�
		if (ch == '\\')
		{
			reader->moveToNextChar(false);
			ch = reader->getChar();
			if (isspace(ch))
				continue;
		}
		// ������ĸ���֣�ֱ���ƽ�
		if (yuki_isWordChar(ch))
		{
			lastCharIsWordChar = true;
			m_matchedName += ch;
		}
		else if (lastCharIsWordChar)
		{
			// ������������ķ��ţ�ȫ��ת���ɵ����� ``-``
			lastCharIsWordChar = false;
			m_matchedName += '-';
		}

		lastCharIsSpace = isspace(ch);
	} while (reader->moveToNextChar(false));

	return false;
}
