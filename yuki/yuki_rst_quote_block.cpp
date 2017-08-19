#include "stdafx.h"
#include "yuki.h"
#include "yuki_struct.h"
#include "yuki_body.h"
#include "yuki_quote_block.h"
#include "yuki_rst_quote_block.h"

/*
	rst 引用块解析要点：

	引用块结束的条件：

	1. 缩进小于等于上一级缩进
	2. 遇到 ``--`` 行

	1. 搜索引用块的范围，统计公共最小缩进
	2. 以公共最小缩进作为引用块的缩进，按照 body 解析引用块

	这个结构本身只是提供框架，不移动字符流指针
*/
void YukiRstQuoteBlock::parse(YukiStruct* parent)
{
	m_parent = parent;

	searchingBlockRegion();

	if(!m_bodyRegion.invalid())
		appendChildByRegion(new YukiBody(m_fileLoader, m_quoteBlockIndent), &m_bodyRegion);

	if (!m_attrRegion.invalid())
		appendChildByRegion(new YukiQuoteBlockAttribute(m_fileLoader, m_quoteBlockIndent), &m_attrRegion);
}

void YukiRstQuoteBlock::searchingBlockRegion()
{
	YukiString line;
	int offset = 0;
	bool lastLineIsBlankLine = false;
	bool foundAttrRegion = false;
	int indentSize = INT_MAX;

	int lineNum = m_fileLoader->getLineNum();
	m_bodyRegion.startLineNum = lineNum;
	m_bodyRegion.endLineNum = m_bodyRegion.startLineNum - 1;

	for (;; ++offset)
	{
		if(outOfRegion())
			break;

		line = m_fileLoader->getLine(offset);

		if (line.invalid())
			break;

		if (line.isBlankLine())
		{
			lastLineIsBlankLine = true;
			// Attribute 后面出现的空行意味着引用块结束
			if (foundAttrRegion)
			{
				++m_attrRegion.endLineNum;
				break;
			}
			++m_bodyRegion.endLineNum;
			continue;
		}
		else
		{
			lastLineIsBlankLine = false;
		}

		// 统计缩进
		int curLineIndent = line.getIndentLevel();

		// 直接统计 Attribute 的范围
		if (foundAttrRegion)
		{
			if(curLineIndent != indentSize)
				break;

			++m_attrRegion.endLineNum;
			continue;
		}

		// 缩进变化，退出结构
		if(curLineIndent <= m_indentLevel)
			break;
		indentSize = yuki_min(indentSize, curLineIndent);

		// 统计 body 部分范围
		// 检测是否出现 attribute 部分
		if (lastLineIsBlankLine && curLineIndent == indentSize && line.matchQuoteBlockAttrMark())
		{
			foundAttrRegion = true;
			m_attrRegion.startLineNum = lineNum + offset;
			m_attrRegion.endLineNum = m_attrRegion.startLineNum - 1;
		}
		else
		{
			++m_bodyRegion.endLineNum;
		}
	}

	m_bodyRegion.indent = m_attrRegion.indent = m_quoteBlockIndent = indentSize;
	assert(indentSize != INT_MAX);
}
