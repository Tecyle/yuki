#include "stdafx.h"
#include "yuki.h"
#include "yuki_quote_block.h"

/*
	当进入解析函数的时候，说明已经成功识别解析了
	解析要点：

	1. 跳过空行
	2. 跳过 ``--`` 打头的文字段
	3. 按照 paragraph 解析
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
