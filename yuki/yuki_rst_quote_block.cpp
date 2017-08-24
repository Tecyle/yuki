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
bool YukiRstQuoteBlock::parse(YukiStruct* parent)
{
	m_parent = parent;

	// ȷ�����ÿ��λ��
	searchingBlockRegion();

	if(!m_bodyRegion.invalid())
		appendChild(new YukiBody(m_fileLoader, &m_bodyRegion));

	if (!m_attrRegion.invalid())
		appendChild(new YukiQuoteBlockAttribute(m_fileLoader, m_attrRegion));
}

/*
	������ǰ���ÿ�ķ�Χ���ֽ������ Region��

	1. BodyRegion�����ÿ����岿�ֵ�λ��
	2. AttributeRegion�����ÿ� Attribute ��Χ��λ��
*/
void YukiRstQuoteBlock::searchingBlockRegion()
{
	const YukiString* line = nullptr;
	int offset = 0;
	bool lastLineIsBlankLine = false;
	bool foundAttrRegion = false;
	int indentSize = INT_MAX;

	int lineNum = m_fileLoader->getLineNum();
	m_bodyRegion.type = m_limitRegion->type;
	m_bodyRegion.startLineNum = m_limitRegion->startLineNum;
	m_bodyRegion.startColNum = m_limitRegion->startColNum;

	// �� region ��Χ���������ÿ�Ľ���λ��
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
		int curLineIndent = line->getIndentLevel().colOffset;

		// ֱ��ͳ�� Attribute �ķ�Χ
		if (foundAttrRegion)
		{
			// Attribute ��Ҫ���㣬�е�����������󹫹�����
			// ��������㣬����Ϊ Attribute �����
			if(curLineIndent != indentSize)
				break;

			++m_attrRegion.endLineNum;
			continue;
		}

		// ���ĳһ�е����������ÿ������ҪС����������ÿ�
		if(curLineIndent <= m_indentLevel)
			break;
		// ��⵽ĿǰΪֹ�����ֵ���󹫹�����
		indentSize = yuki_min(indentSize, curLineIndent);

		// ͳ�� body ���ַ�Χ
		// ����Ƿ���� attribute ����
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
