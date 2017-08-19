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

	// 确定引用块的位置
	searchingBlockRegion();

	if(!m_bodyRegion.invalid())
		appendChildByRegion(new YukiBody(m_fileLoader, m_quoteBlockIndent), &m_bodyRegion);

	if (!m_attrRegion.invalid())
		appendChildByRegion(new YukiQuoteBlockAttribute(m_fileLoader, m_quoteBlockIndent), &m_attrRegion);
}

/*
	搜索当前引用块的范围，分解成两个 Region：

	1. BodyRegion：引用块主体部分的位置
	2. AttributeRegion：引用块 Attribute 范围的位置
*/
void YukiRstQuoteBlock::searchingBlockRegion()
{
	const YukiString* line = nullptr;
	int offset = 0;
	bool lastLineIsBlankLine = false;
	bool foundAttrRegion = false;
	int indentSize = INT_MAX;

	int lineNum = m_fileLoader->getLineNum();
	m_bodyRegion.startLineNum = lineNum;
	m_bodyRegion.endLineNum = m_bodyRegion.startLineNum - 1;

	// 在 region 范围内搜索引用块的结束位置
	for (;; ++offset)
	{
		if(outOfRegion())
			break;

		line = m_fileLoader->getLine(offset);

		if (line == nullptr)
			break;

		if (line->isBlankLine())
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
		int curLineIndent = line->getIndentLevel().colOffset;

		// 直接统计 Attribute 的范围
		if (foundAttrRegion)
		{
			// Attribute 需要满足，行的缩进等于最大公共缩进
			// 如果不满足，则认为 Attribute 块结束
			if(curLineIndent != indentSize)
				break;

			++m_attrRegion.endLineNum;
			continue;
		}

		// 如果某一行的缩进比引用块的缩进要小，则结束引用块
		if(curLineIndent <= m_indentLevel)
			break;
		// 求解到目前为止，出现的最大公共缩进
		indentSize = yuki_min(indentSize, curLineIndent);

		// 统计 body 部分范围
		// 检测是否出现 attribute 部分
		if (lastLineIsBlankLine && curLineIndent == indentSize && line->matchQuoteBlockAttrMark())
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
