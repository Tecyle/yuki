#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_simple_reference_name.h"

const wchar_t* g_dividerSet = L"-._+:";

bool YukiSimpleReferenceName::parseReferenceName(wstring* res)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	bool succ = false;

	while (parseWordBlock(res))
	{
		succ = true;
		wchar_t ch = reader->getChar();
		if (wcschr(g_dividerSet, ch) == nullptr || !yuki_isWordChar(reader->getChar(1)))
		{
			break;
		}
		if (res)
			*res += ch;
		reader->moveToNextChar(false);
	}

	if (!succ)
		reader->setCursor(oldCursor);
	return succ;
}

bool YukiSimpleReferenceName::parseWordBlock(wstring* res)
{
	YukiFileReader* reader = getFileReader();
	
	wchar_t ch = reader->getChar();
	if (!yuki_isWordChar(ch))
		return false;

	if (res)
		res += ch;
	while (reader->moveToNextChar(false))
	{
		ch = reader->getChar();
		if (!yuki_isWordChar(ch))
			break;

		if (res)
			res += ch;
	}

	return true;
}
