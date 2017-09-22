#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_md_quote_block.h"
#include "yuki_simple_reference_name.h"

bool YukiMdQuoteBlock::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const YukiRegion* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!matchNoBackward())
		goto match_finished;

	int commonIndent = INT_MAX;
	yuki_cursor lineBeginCursor = reader->getCursor();
	oldCursor = reader->getCursor();
	if (m_quoteLevel.empty())
		commonIndent = reader->skipSpaces();
	while (reader->moveToNextLine())
	{
		lineBeginCursor = reader->getCursor();
		if (!reader->matchChar('>'))
			break;
		
		int indent = reader->skipSpaces();
		if (indent < 1)
			break;

		commonIndent = yuki_min(commonIndent, indent);
		// TODO ¼ì²â Attribute
	}

	YukiQuoteBlockNode* node = new YukiQuoteBlockNode;
	reader->setCursor(oldCursor);
	node->setQuoteCategory(m_quoteLevel);
	getParser(L"body")->parse(node, reader->cutRegionBetween(oldCursor, lineBeginCursor, commonIndent + 1));
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
