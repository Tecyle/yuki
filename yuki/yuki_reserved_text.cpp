#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_struct.h"
#include "yuki_node.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_reserved_text.h"

bool YukiReservedText::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	const yuki_region* oldRegion = reader->selectRegion(region);
	
	YukiReservedTextNode* node = new YukiReservedTextNode;
	wstring& rsvText = node->reservedText();
	do 
	{
		const YukiLineString* line = reader->getLine();
		rsvText.append(line->getContentPtr(), line->getContentLength());
	} while (reader->moveToNextLine());

	parentNode->appendChild(node);
	reader->selectRegion(oldRegion);
	return true;
}
