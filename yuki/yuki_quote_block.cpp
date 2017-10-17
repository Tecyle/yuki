#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_quote_block.h"
#include "yuki_quote_block_attribute_node.h"


yuki_quote_block_attribute::yuki_quote_block_attribute(yuki_session* session)
	: yuki_structure_parser(session)
{
	m_name = yuki_quote_block_attribute_name;
	m_type = yuki_blockStructure;
}


/*
	解析要点：

	1. 跳过空行
	2. 跳过 ``--`` 打头的文字段
	3. 按照 inline_block 解析
*/
bool yuki_quote_block_attribute::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	if (!matchNoBackward())
	{
		reader->setCursor(oldCursor);
		return false;
	}

	yuki_quote_block_attribute_node* attrNode = new yuki_quote_block_attribute_node;
	getParser(yuki_inline_block_name)->parse(attrNode, reader->cutRegionFromCursorToEnd());
	parentNode->appendChild(parentNode);
	return true;
}

/*
	在 match 的时候，仅判断前缀，不判断缩进相关
	缩进相关的应该在 quote block 那一层判断
*/
bool yuki_quote_block_attribute::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool yuki_quote_block_attribute::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();

	reader->skipSpaces();
	if (!reader->matchStr(L"---")
		&& !reader->matchStr(L"--")
		&& !reader->matchStr(L"——"))
	{
		return false;
	}

	if (reader->skipSpaces() <= 0)
		return false;

	if (reader->cursorAtLineEnd())
		return false;

	return true;
}
