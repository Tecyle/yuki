#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_md_literal_block.h"

static bool yuki_isLanguageNameChar(wchar_t ch)
{
	return !isspace(ch) && ch != '`';
}

bool YukiMdLiteralBlock::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiMdLiteralBlock::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();

	if (!reader->matchStr(L"```"))
		return false;

	wchar_t ch = reader->getChar();
	m_language.clear();
	while (yuki_isLanguageNameChar(ch))
	{
		m_language += ch;
		if (!reader->moveToNextChar(false))
			break;
	}
	

}
