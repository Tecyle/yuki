#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_rst_quote_block.h"
#include "yuki_session.h"
#include "yuki_body.h"
#include "yuki_file_reader.h"
#include "yuki_body_node.h"
#include "yuki_structure_parser_collection.h"
#include "yuki_line_string.h"

/*
	Body 节点下面允许出现以下节点：

	1. 有序列表
	2. 无序列表
	3. 字段列表
	4. 选项列表
	5. 定义列表
	6. 行列表
	7. 预排文字块
	8. Markdown 预排文字块
	9. 引用块
	10. Markdown 引用块
	11. doctest 块
	12. 显示标记块
	13. 网格表
	14. 简单表
	15. Markdown 表
	16. 分割线
	17. 段落

	各个节点的特征为：

	1. 满足有序列表的序号要求，并且后面存在空格 √
	2. 以 ``*``, ``+`` 或 ``-`` 打头，且后面存在空格 √
	3. 满足 ``:filed:`` 形式打头，存在空格 √
	4. 满足参数打头，存在至少两个空格 √
	5. 下一行不是空且存在缩进 √
	6. 以 ``|`` 打头且存在空格 √
	7. 以 ``::`` 打头且存在空格 √
	8. 以 ``` 打头 √
	9. 存在相对缩进 √
	10. 以 ``>`` 打头并存在空格或中括号 √
	11. 以 ``>>>`` 打头并存在空格 √
	12. 以 ``..`` 打头并存在空格 √
	13. 满足网格表头部 √
	14. 满足简单表头部 √
	15. 如果行内存在 ``|``，向下寻找标题行，如果找到不存在 ``|`` 的行或者空行，则匹配失败 √
	16. 连续4个相同标点打头 √
	17. 其余的为段落

	根据以上特征，识别顺序如下：

	1. 如果缩进打头，识别为引用块 √
	2. 如果以 ``.. `` 打头，则识别为显式标记块 √
	3. 如果以 ``` 打头，则识别为 Markdown 预排文字块 √
	4. 如果以 ``>>> ``打头，则识别为 doctest 块 √
	5. 如果以 ``>`` 打头，则识别为 Markdown 引用块 √
	6. 如果以 ``::`` 打头，并且这一行和下一行只有空格，则识别为预排文字块
	7. 无序列表标记打头，识别为无序列表
	8. 有序列表标记打头，识别为有序列表
	9. field 参数打头，识别为参数列表
	10. 连续四个标点打头，如果存在其它文字，以同样四个标点结束，识别为分割线
	11. 匹配网格表头
	12. 匹配简单表头
	13. 匹配参数打头
	14. 下一行有缩进，识别为定义列表
	15. 如果行内非行首存在 ``|``，搜索下几行，判断是否是 Markdown 风格表格
	16. 如果以 ``| `` 打头，则识别为行列表
	17. 然后只剩段落了
*/
yuki_body::yuki_body(yuki_session* globalData)
	: yuki_structure_parser(globalData)
{
	m_name = yuki_body_name;
	m_type = yuki_blockStructure;
}


bool yuki_body::parse(yuki_node* parent, const yuki_region* region)
{
	int indentLevel = region->getIndent();
	yuki_file_reader* fileReader = getFileReader();
	fileReader->pushRegion(region);

	yuki_body_node* bodyNode = new yuki_body_node();
	auto followSet = m_session->getStructureParserCollection()->getFollowParsersByName(getName());

	while (!fileReader->outOfRegion())
	{
		const yuki_line_string* line = fileReader->getLine();

		if (line == nullptr)
			break;

		if (line->isBlankLine())
		{
			fileReader->moveToNextLine();
			continue;
		}

		if (line->getIndent() < region->getIndent())
			break;

		bool matched = false;
		for (auto parser : *followSet)
		{
			assert(parser);
			if (!parser->match())
				continue;

			matched = parser->parse(parent, region);
			break;
		}

		assert(matched);
	}

	fileReader->popRegion();
	parent->appendChild(bodyNode);

	return true;
}
