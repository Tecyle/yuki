#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_hyperlink_target.h"
#include "yuki_link_uri.h"

bool YukiLinkUri::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	const yuki_region* oldRegion = reader->selectRegion(region);

	YukiHyperlinkTargetNode* node = dynamic_cast<YukiHyperlinkTargetNode*>(parentNode);
	assert(node);

	// ����������ĩ�ַ�
	yuki_cursor cursor = reader->getCursor();
	bool aliasMode = false;
	wchar_t ch = 0;
	do 
	{
		wchar_t newChar = reader->getChar();
		// �����ĵ�һ���ַ�����Ƿ����ţ����� alias ����
		if (ch == 0 && newChar == '`')
		{
			aliasMode = true;
			break;
		}
		// ��һ���ַ���б�ܣ���ô�������ַ���ת���ַ�
		newChar = ch == '\\' ? 'y' : reader->getChar();
		if (isspace(newChar))
			continue;

		if (ch == 0)
		{
			cursor = reader->getCursor();
		}
		ch = newChar;
		aliasMode = ch =='_';
	} while (reader->moveToNextChar());

	if (ch == 0)
	{
		// ���û�������κηǿ��ַ�
		reader->selectRegion(oldRegion);
		return false;
	}
	// ���� uri��ע�⣬��ȥ��� uri ����ȷ��
	reader->setCursor(cursor);
	if (aliasMode)
		parseAliasTarget(node);
	else
		parseUrlTarget(node);

	reader->selectRegion(oldRegion);
	return true;
}

void YukiLinkUri::parseAliasTarget(YukiHyperlinkTargetNode* node)
{
	YukiFileReader* reader = getFileReader();
	node->setTargetType(HyperlinkTarget_alias);
	// ��һ���ַ�Ϊ��Ч�ַ����ж��Ƿ��� �����ţ��Ӷ������������ͼ�����
	bool hasQuote = reader->getChar() == '`';
	if (hasQuote)
		reader->moveToNextChar();
	
	wstring& target = node->uri();
	bool isLastCharNotWord = false;
	do 
	{
		wchar_t ch = reader->getChar();
		
		if (ch == '\\')
		{
			// ת���ַ���ԭ������
			reader->moveToNextChar();
			ch = reader->getChar();
			target += ch;
			isLastCharNotWord = !yuki_isWordChar(ch);
			continue;
		}

		if (yuki_isWordChar(ch))
		{
			// �ַ�����ֱ�Ӹ���
			target += ch;
			isLastCharNotWord = false;
			continue;
		}

		// �������������
		if (hasQuote)
		{
			if (ch == '`')
			{
				reader->moveToNextChar();
				break;
			}
		}
		else if (ch == '_')
			break;

		// �����ַ���������ɼ򵥵ĺ���
		if (!isLastCharNotWord)
		{
			target += '-';
			isLastCharNotWord = true;
		}
	} while (reader->moveToNextChar());
}

void YukiLinkUri::parseUrlTarget(YukiHyperlinkTargetNode* node)
{
	YukiFileReader* reader = getFileReader();
	node->setTargetType(HyperlinkTarget_external);
	// �հ��ַ����ԣ�ת���ַ�ԭ�����
	wstring& target = node->uri();
	do 
	{
		wchar_t ch = reader->getChar();

		if (isspace(ch))
			continue;

		if (ch == '\\')
		{
			reader->moveToNextChar();
			ch = reader->getChar();
		}

		target += ch;
	} while (reader->moveToNextChar());
}
