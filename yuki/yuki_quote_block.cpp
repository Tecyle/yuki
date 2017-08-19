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

	m_fileLoader->skipBlankLinesInRegion(m_limitRegion);

	if (outOfRegion())
		return;

	if (!m_fileLoader->match(L"---") && !m_fileLoader->match(L"--"))
		assert(!"Failed match quote block attribute!");
	m_fileLoader->matchAndSkipSpace();

	YukiBlockRegion region;
	region.startLineNum = m_fileLoader->getLineNum() - 1;
	region.endLineNum = m_limitRegion->endLineNum;
	region.indent = m_indentLevel;
	appendChildByRegion(new YukiParagraph(m_fileLoader, m_indentLevel), &region);
}
