#include "stdafx.h"
#include "yuki.h"
#include "yuki_struct.h"
#include "yuki_body.h"
#include "yuki_quote_block.h"
#include "yuki_rst_quote_block.h"

/*
	rst ���ÿ����Ҫ�㣺

	���ÿ������������

	1. ����С�ڵ�����һ������
	2. ���� ``--`` ��

	1. �������ÿ�ķ�Χ��ͳ�ƹ�����С����
	2. �Թ�����С������Ϊ���ÿ������������ body �������ÿ�

	����ṹ����ֻ���ṩ��ܣ����ƶ��ַ���ָ��
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
			// Attribute ������ֵĿ�����ζ�����ÿ����
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

		// ͳ������
		int curLineIndent = line.getIndentLevel();

		// ֱ��ͳ�� Attribute �ķ�Χ
		if (foundAttrRegion)
		{
			if(curLineIndent != indentSize)
				break;

			++m_attrRegion.endLineNum;
			continue;
		}

		// �����仯���˳��ṹ
		if(curLineIndent <= m_indentLevel)
			break;
		indentSize = yuki_min(indentSize, curLineIndent);

		// ͳ�� body ���ַ�Χ
		// ����Ƿ���� attribute ����
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
