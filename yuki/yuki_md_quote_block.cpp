#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_md_quote_block.h"
#include "yuki_simple_reference_name.h"

bool YukiMdQuoteBlock::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);
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

		// 如果某一行的缩进等于最大公共缩进，并且匹配 Attribute，则结束 body 部分
		if (indent == commonIndent && getParser(L"quote_block_attribute")->match())
		{
			hasAttr = true;
			attrCursor = reader->getCursor();
			break;
		}
		
		commonIndent = yuki_min(commonIndent, indent);
	}

	YukiQuoteBlockNode* node = new YukiQuoteBlockNode;
	reader->setCursor(oldCursor);
	node->setQuoteCategory(m_quoteLevel);
	getParser(L"body")->parse(node, reader->cutRegionBetween(oldCursor, lineBeginCursor, commonIndent + 1));
	if (hasAttr)
	{
		reader->setCursor(attrCursor);
		// 继续搜索 Attribute 的位置
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
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiMdQuoteBlock::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	
	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiMdQuoteBlock::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();
	
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
