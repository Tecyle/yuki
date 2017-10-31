#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_md_quote_block.h"
#include "yuki_simple_reference_name.h"
#include "yuki_quote_block_node.h"

bool yuki_md_quote_block::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	reader->pushRegion(region);
	bool succ = false;

	if (!matchNoBackward())
		goto match_finished;

	int commonIndent = INT_MAX;
	yuki_cursor lineBeginCursor = reader->getCursor();
	yuki_cursor attrCursor;
	oldCursor = reader->getCursor();
	if (m_quoteLevel.empty())
		commonIndent = reader->skipSpaces();

	bool hasAttr = false;
	while (reader->moveToNextLine())
	{
		lineBeginCursor = reader->getCursor();
		if (!reader->matchChar('>'))
			break;
		
		int indent = reader->skipSpaces();
		if (indent < 1)
			break;

		// ���ĳһ�е�����������󹫹�����������ƥ�� Attribute������� body ����
		if (indent == commonIndent && getParser(L"quote_block_attribute")->match())
		{
			hasAttr = true;
			attrCursor = reader->getCursor();
			break;
		}
		
		commonIndent = yuki_min(commonIndent, indent);
	}

	yuki_quote_block_node* node = new yuki_quote_block_node;
	reader->setCursor(oldCursor);
	node->setQuoteType(m_quoteLevel);
	getParser(L"body")->parse(node, reader->cutRegionBetween(oldCursor, lineBeginCursor, commonIndent + 1));
	if (hasAttr)
	{
		reader->setCursor(attrCursor);
		// �������� Attribute ��λ��
		lineBeginCursor = attrCursor;
		while (reader->moveToNextLine())
		{
			lineBeginCursor = reader->getCursor();
			if (!reader->matchChar('>'))
				break;

			int indent = reader->skipSpaces();
			if (indent != commonIndent)
				break;
		}
		reader->setCursor(attrCursor);
		getParser(L"quote_block_attribute")->parse(node, reader->cutRegionFromCursorTo(lineBeginCursor, commonIndent + 1));
	}
	succ = true;
	parentNode->appendChild(node);

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->popRegion();
	return succ;
}

bool yuki_md_quote_block::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	
	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool yuki_md_quote_block::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();
	
	m_quoteLevel.empty();

	if (!reader->matchChar('>'))
		return false;
	if (reader->matchChar('['))
	{
		YukiSimpleReferenceName* referenceNameParser = dynamic_cast<YukiSimpleReferenceName*>(getParser(L"simple_reference_name"));
		if (!referenceNameParser->parseReferenceName(&m_quoteLevel))
			return false;

		if (!reader->matchChar(']'))
			return false;

		if (reader->skipSpaces() < 1)
			return false;
	}
	else
	{
		wchar_t ch = reader->getChar();
		if (!isspace(ch))
			return false;
	}

	return true;
}
