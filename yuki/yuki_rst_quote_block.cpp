#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
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
bool YukiRstQuoteBlock::parse(YukiNode* parentNode, const yuki_region* region)
{
	if (!match())
		return false;

	YukiQuoteBlockNode* quoteNode = new YukiQuoteBlockNode;
	const yuki_region* bodyRegion;
	const yuki_region* attrRegion;

	quoteNode->setQuoteLevel(QuoteLevel_default);
	// ȷ�����ÿ��λ��
	searchingBlockRegion(bodyRegion, attrRegion);

	assert(bodyRegion != nullptr || attrRegion != nullptr);

	if (bodyRegion != nullptr)
		getParser(L"body")->parse(quoteNode, bodyRegion);

	if (attrRegion != nullptr)
		getParser(L"quote_block_attribute")->parse(quoteNode, attrRegion);
		
	parentNode->appendChild(quoteNode);
	return true;
}


bool YukiRstQuoteBlock::match()
{
	yuki_file_reader* reader = getFileReader();
	const yuki_region* region = reader->getRegion();

	const yuki_line_string* line = reader->getLine();
	assert(!line->isBlankLine());

	return line->getIndent() > region->getIndent();
}

/*
	������ǰ���ÿ�ķ�Χ���ֽ������ Region��

	1. BodyRegion�����ÿ����岿�ֵ�λ��
	2. AttributeRegion�����ÿ� Attribute ��Χ��λ��
*/
void YukiRstQuoteBlock::searchingBlockRegion(const yuki_region* &bodyRegion, const yuki_region* &attrRegion)
{
	yuki_file_reader* reader = getFileReader();
	int indent = reader->getRegion()->getIndent();
	int commonIndent = INT_MAX;	///< ͳ�� body ���ֵ���󹫹�����
	bool lastLineIsBlankLine = true;
	bool hasAttr = false;
	yuki_cursor oldCursor = reader->getCursor();
	yuki_cursor startCursor = oldCursor;
	bodyRegion = nullptr;
	attrRegion = nullptr;

	// ���� body ����
	do
	{
		const yuki_line_string* line = reader->getLine();
		// ��ȡ������Χ������Ϊֻ�� body ����
		if (line == nullptr)
		{
			bodyRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
			goto search_finished;
		}
		// ��������˿���
		if (line->isBlankLine())
		{
			lastLineIsBlankLine = true;
			continue;
		}
		// �����������С�ڵ�ǰ�������У�����Ϊ quote block ����
		if (line->getIndent() < indent)
		{
			bodyRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
			goto search_finished;
		}
		// ͳ����󹫹�����
		commonIndent = yuki_min(commonIndent, line->getIndent());
		if (lastLineIsBlankLine)
		{
			// ���ĳһ�е�����������󹫹�����������ƥ�� Attribute������� body ����
			if (line->getIndent() == commonIndent && getParser(L"quote_block_attribute")->match())
			{
				bodyRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
				startCursor = reader->getCursor();
				hasAttr = true;
				break;
			}
			lastLineIsBlankLine = false;
		}
	} while (reader->moveToNextLine());

	// ���� attribute ����
	while (reader->moveToNextLine())
	{
		// �ܽ������˵������� attr ��ǩƥ��ɹ���
		// ���ԣ�ֻ���ƽ�����ƥ�����
		const yuki_line_string* line = reader->getLine();

		if (line == nullptr || line->isBlankLine() || line->getIndent() != commonIndent)
		{
			attrRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
			goto search_finished;
		}
	}
	// δ������������������
	if (hasAttr)
	{
		attrRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
	}

search_finished:
	reader->setCursor(oldCursor);
}
