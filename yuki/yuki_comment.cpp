#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_comment.h"
#include "yuki_comment_node.h"

bool yuki_comment::parse(yuki_node* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_comment_node* node = new yuki_comment_node;

	// 注释直接按预留文字处理
	getParser(L"reserved_text")->parse(node, region);

	parentNode->appendChild(node);
	return true;
}
