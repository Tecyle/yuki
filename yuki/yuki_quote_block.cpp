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
