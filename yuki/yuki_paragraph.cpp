#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_paragraph.h"
#include "yuki_paragraph_node.h"

bool yuki_paragraph::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	reader->pushRegion(region);
	yuki_cursor oldCursor = reader->getCursor();

	while (!reader->matchBlankLine())
	{
		if(!reader->moveToNextLine())
			break;
	}

	yuki_paragraph_node* node = new yuki_paragraph_node;
	const yuki_region* paragraphRegion = reader->cutRegionToCursorFrom(oldCursor);
	reader->setCursor(oldCursor);
	getParser(L"inline_block")->parse(node, paragraphRegion);
	parentNode->appendChild(node);

	reader->popRegion();
	return true;
}
