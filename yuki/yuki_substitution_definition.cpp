#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_substitution_definition.h"
#include "yuki_simple_reference_name.h"

bool YukiSubstitutionDefinition::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!reader->matchChar('|'))
		goto match_finished;

	YukiSimpleReferenceName* simpleRefName = dynamic_cast<YukiSimpleReferenceName*>(getParser(L"simple_reference_name"));
	wstring refName;
	if (!simpleRefName->parseReferenceName(&refName))
		goto match_finished;

	if (!reader->matchChar('|'))
		goto match_finished;

	if (reader->skipSpaces() < 1)
		goto match_finished;

	succ = true;
	YukiSubstitutionDefinitionNode* node = new YukiSubstitutionDefinitionNode;
	getParser(L"directives")->parse(node, reader->cutRegionFromCursorToEnd());
	parentNode->appendChild(node);

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->selectRegion(oldRegion);
	return succ;
}
