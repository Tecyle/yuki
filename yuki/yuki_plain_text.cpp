#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_node.h"
#include "yuki_struct.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_plain_text.h"

/*
	plain text ����Ҫ�㣺

	1. region ��Χ���������־�����
	2. ����ÿһ�е�����
	3. ����հ��ַ�����һ���ո�
	4. ת���ַ�����
	5. �����趨������β��������ת��
*/
static const wchar_t* g_enterFollowSet = L"abcdefghijklmnopqrstuvwxyz";

bool YukiPlainText::parse(yuki_node* parentNode, const yuki_region* region)
{
	if (region->empty())
		return true;

	yuki_file_reader* reader = getFileReader();
	const yuki_region* oldRegion = reader->selectRegion(region);
	YukiPlainTextNode* textNode = new YukiPlainTextNode;
	wstring& text = textNode->plainText();
	wchar_t lastChar = 0;

	for (; reader->moveToNextChar();)
	{
		wchar_t ch = reader->getChar();

		// ����ǻس���
		if (ch == '\n')
		{
			lastChar = ch;
			continue;
		}

		// ����ǿհ��ַ�
		if (isspace(ch))
		{
			// ת��հף����Ӧ����ʲô�������
			if (lastChar == '\\')
			{
				lastChar = 0;
				continue;
			}
			// ��������հ�ֻ���һ��
			if (lastChar != ' ')
			{
				text += ' ';
				lastChar = ' ';
			}
			continue;
		}

		// �����ת����ʼ�ַ�
		if (ch == '\\' && lastChar != '\\')
		{
			lastChar = '\\';
			continue;
		}

		// �����ַ���ԭ�����
		if (lastChar == '\n' && wcschr(g_enterFollowSet, tolower(ch)) != nullptr)
			text += ' ';

		text += ch;
		lastChar = ch;
	}

	parentNode->appendChild(textNode);
	reader->selectRegion(oldRegion);
	return true;
}
