#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_simple_reference_name.h"

/*
	识别脚注或者是引用标注的要点：

	1. ``[`` 打头
	2. 后跟以下内容：
		1. 纯数字，属于脚注
		2. # 后面跟或者不跟简单引用名称，属于脚注
		3. 单个 *，属于脚注
		4. 引用名称，属于引用标注
	3. 后跟 ``]`` 加空格

	则识别成功
*/
bool YukiFootnoteCitationDefinition::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!reader->matchChar('['))
		goto match_finished;

	if (!reader->moveToNextChar(false))
		goto match_finished;
	
	wstring label;
	bool isFootnote;
	if (!parseLabel(label, isFootnote))
		goto match_finished;

	if (!reader->matchChar(']'))
		goto match_finished;

	if (!reader->moveToNextChar(false))
		goto match_finished;

	if (reader->skipSpaces() == 0 && !reader->cursorAtLineEnd())
		goto match_finished;

	YukiNode* node;
	if (isFootnote)
	{
		node = new YukiFootnoteDefinitionNode;
	}
	else
	{
		node = new YukiCitationDefinitionNode;
	}
	parentNode->appendChild(node);
	getParser(L"body")->parse(node, reader->cutRegionFromCursorToEnd());

	succ = true;

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiFootnoteCitationDefinition::parseLabel(wstring& label, bool& isFootnote)
{
	yuki_file_reader* reader = getFileReader();
	wstring number;
	YukiSimpleReferenceName* simpleRefName = dynamic_cast<YukiSimpleReferenceName*>(getParser(L"simple_reference_name"));
	wchar_t ch = reader->getChar();
	if (!reader->moveToNextChar(false))
		return false;

	if (ch == '*')
	{
		if (reader->getChar() != ']')
			return false;

		label = L"*";
		isFootnote = true;
		return true;
	}

	if (ch == '#')
	{
		label = L"#";
		if (reader->getChar() != ']' && !simpleRefName->parseReferenceName(&label))
			return false;

		isFootnote = true;
		return true;
	}

	if (isdigit(ch))
	{
		// 尝试解析数字
		while (isdigit(ch))
		{
			number += ch;
			if (!reader->moveToNextChar(false))
				return false;
			ch = reader->getChar();
		}

		if (ch == ']')
		{
			// 解析数字成功
			isFootnote = true;
			label = number;
			return true;
		}
		// 解析数字失败，则继续解析简单引用名称
	}

	if (!simpleRefName->parseReferenceName(&label))
		return false;

	label = number + label;
	isFootnote = false;
	return true;
}
