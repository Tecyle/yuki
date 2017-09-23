#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_paragraph.h"

bool YukiParagraph::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	const yuki_region* oldRegion = reader->selectRegion(region);
	yuki_cursor oldCursor = reader->getCursor();

	while (!reader->matchBlankLine())
	{
		if(!reader->moveToNextLine())
			break;
	}

	YukiParagraphNode* node = new YukiParagraphNode;
	const yuki_region* paragraphRegion = reader->cutRegionToCursorFrom(oldCursor);
	reader->setCursor(oldCursor);
	getParser(L"inline_block")->parse(node, paragraphRegion);
	parentNode->appendChild(node);

	reader->selectRegion(oldRegion);
	return true;
}
