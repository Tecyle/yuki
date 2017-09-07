#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_hyperlink_target.h"
#include "yuki_link_uri.h"

bool YukiLinkUri::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	const YukiRegion* oldRegion = reader->selectRegion(region);

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
	node->setTargetType(HyperlinkTarget_alias);
	// ��һ���ַ�Ϊ��Ч�ַ����ж��Ƿ��� �����ţ��Ӷ������������ͼ�����

}
