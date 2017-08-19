#include "stdafx.h"
#include "yuki.h"
#include "yuki_quote_block.h"

/*
	���������������ʱ��˵���Ѿ��ɹ�ʶ�������
	����Ҫ�㣺

	1. ��������
	2. ���� ``--`` ��ͷ�����ֶ�
	3. ���� paragraph ����
*/
void YukiQuoteBlockAttribute::parse(YukiStruct* parent)
{
	m_parent = parent;

	for (;;)
	{
		if (outOfRegion())
			return;

		YukiString line = m_fileLoader->getLine();
		if (line.isBlankLine())
		{
			m_fileLoader->moveToNextLine();
			continue;
		}

		if (m_fileLoader->match("---") || m_fileLoader->match("--"))
		{
			assert(!"Failed match quote block attribute!");
		}
		m_fileLoader->matchAndSkipSpace();

		YukiBlockRegion region;
		region.startLineNum = m_fileLoader->getLineNum();
		region.endLineNum = m_limitRegion->endLineNum;
		region.indent = m_indentLevel;
		appendChildByRegion(new YukiParagraph(m_fileLoader, m_indentLevel), &region);

		break;
	}
}
