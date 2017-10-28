#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_structure_parser.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_simple_reference_name.h"
#include "yuki_footnote_definition_node.h"
#include "yuki_citation_definition_node.h"

/*
	ʶ���ע���������ñ�ע��Ҫ�㣺

	1. ``[`` ��ͷ
	2. ����������ݣ�
		1. �����֣����ڽ�ע
		2. # ��������߲������������ƣ����ڽ�ע
		3. ���� *�����ڽ�ע
		4. �������ƣ��������ñ�ע
	3. ��� ``]`` �ӿո�

	��ʶ��ɹ�
*/
bool yuki_footnote_citation_definition::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	reader->pushRegion(region);
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

	yuki_node* node;
	if (isFootnote)
	{
		node = new yuki_footnote_definition_node;
	}
	else
	{
		node = new yuki_citation_definition_node;
	}
	parentNode->appendChild(node);
	getParser(L"body")->parse(node, reader->cutRegionFromCursorToEnd());

	succ = true;

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->popRegion();
	return succ;
}

bool yuki_footnote_citation_definition::parseLabel(wstring& label, bool& isFootnote)
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
		// ���Խ�������
		while (isdigit(ch))
		{
			number += ch;
			if (!reader->moveToNextChar(false))
				return false;
			ch = reader->getChar();
		}

		if (ch == ']')
		{
			// �������ֳɹ�
			isFootnote = true;
			label = number;
			return true;
		}
		// ��������ʧ�ܣ��������������������
	}

	if (!simpleRefName->parseReferenceName(&label))
		return false;

	label = number + label;
	isFootnote = false;
	return true;
}
