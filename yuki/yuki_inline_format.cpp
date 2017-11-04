#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_markup.h"
#include "yuki_inline_format.h"

bool yuki_inline_format::parse(yuki_node* parentNode, const yuki_region* region)
{
	if (!match())
		return false;

	yuki_file_reader* reader = getFileReader();
	yuki_node* node = allocNode();
	reader->setCursor(m_textRegion1->begin());
	if (m_allowEscapeNearMark)
		getParser(L"reserved_text")->parse(node, m_textRegion1);
	else
		getParser(L"plain_text")->parse(node, m_textRegion1);
	reader->setCursor(m_endCursor);

	parentNode->appendChild(node);
	return true;
}
