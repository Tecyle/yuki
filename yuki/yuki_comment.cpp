#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_comment.h"

bool YukiComment::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	YukiCommentNode* node = new YukiCommentNode;

	// ע��ֱ�Ӱ�Ԥ�����ִ���
	getParser(L"reserved_text")->parse(node, region);

	parentNode->appendChild(node);
	return true;
}
