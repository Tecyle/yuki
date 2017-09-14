#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_directives.h"
#include "yuki_simple_reference_name.h"

bool YukiDirectives::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const YukiRegion* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!matchNoBackward())
		goto match_finished;

	succ = true;
	wstring directive = m_directiveName + +L"-directive";
	YukiStruct* parser = getParser(directive.c_str());
	if (parser != nullptr)
	{
		parser->parse(parentNode, reader->cutRegionFromCursorToEnd());
	}
	else
	{
		getParser(L"reserved_text")->parse(parentNode, reader->cutRegionFromCursorToEnd());
	}

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiDirectives::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiDirectives::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();
	YukiSimpleReferenceName* simpleRefName = dynamic_cast<YukiSimpleReferenceName*>(getParser(L"simple_reference_name"));

	if (!simpleRefName->parseReferenceName(&m_directiveName))
		return false;

	if (!reader->matchStr(L"::"))
		return false;

	if (reader->skipSpaces() < 1)
		return false;

	return true;
}
