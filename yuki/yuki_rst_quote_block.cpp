#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_body.h"
#include "yuki_quote_block.h"
#include "yuki_rst_quote_block.h"

/*
	rst 引用块解析要点：

	引用块结束的条件：

	1. 缩进小于等于上一级缩进
	2. 遇到 ``--`` 行

	1. 搜索引用块的范围，统计公共最小缩进
	2. 以公共最小缩进作为引用块的缩进，按照 body 解析引用块

	这个结构本身只是提供框架，不移动字符流指针
*/
bool YukiRstQuoteBlock::parse(YukiNode* parentNode, const yuki_region* region)
{
	if (!match())
		return false;

	YukiQuoteBlockNode* quoteNode = new YukiQuoteBlockNode;
	const yuki_region* bodyRegion;
	const yuki_region* attrRegion;

	quoteNode->setQuoteLevel(QuoteLevel_default);
	// 确定引用块的位置
	searchingBlockRegion(bodyRegion, attrRegion);

	assert(bodyRegion != nullptr || attrRegion != nullptr);

	if (bodyRegion != nullptr)
		getParser(L"body")->parse(quoteNode, bodyRegion);

	if (attrRegion != nullptr)
		getParser(L"quote_block_attribute")->parse(quoteNode, attrRegion);
		
	parentNode->appendChild(quoteNode);
	return true;
}


bool YukiRstQuoteBlock::match()
{
	yuki_file_reader* reader = getFileReader();
	const yuki_region* region = reader->getRegion();

	const yuki_line_string* line = reader->getLine();
	assert(!line->isBlankLine());

	return line->getIndent() > region->getIndent();
}

/*
	搜索当前引用块的范围，分解成两个 Region：

	1. BodyRegion：引用块主体部分的位置
	2. AttributeRegion：引用块 Attribute 范围的位置
*/
void YukiRstQuoteBlock::searchingBlockRegion(const yuki_region* &bodyRegion, const yuki_region* &attrRegion)
{
	yuki_file_reader* reader = getFileReader();
	int indent = reader->getRegion()->getIndent();
	int commonIndent = INT_MAX;	///< 统计 body 部分的最大公共缩进
	bool lastLineIsBlankLine = true;
	bool hasAttr = false;
	yuki_cursor oldCursor = reader->getCursor();
	yuki_cursor startCursor = oldCursor;
	bodyRegion = nullptr;
	attrRegion = nullptr;

	// 搜索 body 部分
	do
	{
		const yuki_line_string* line = reader->getLine();
		// 获取超出范围，则认为只有 body 部分
		if (line == nullptr)
		{
			bodyRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
			goto search_finished;
		}
		// 标记遇到了空行
		if (line->isBlankLine())
		{
			lastLineIsBlankLine = true;
			continue;
		}
		// 如果遇到缩进小于当前缩进的行，则认为 quote block 结束
		if (line->getIndent() < indent)
		{
			bodyRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
			goto search_finished;
		}
		// 统计最大公共缩进
		commonIndent = yuki_min(commonIndent, line->getIndent());
		if (lastLineIsBlankLine)
		{
			// 如果某一行的缩进等于最大公共缩进，并且匹配 Attribute，则结束 body 部分
			if (line->getIndent() == commonIndent && getParser(L"quote_block_attribute")->match())
			{
				bodyRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
				startCursor = reader->getCursor();
				hasAttr = true;
				break;
			}
			lastLineIsBlankLine = false;
		}
	} while (reader->moveToNextLine());

	// 搜索 attribute 部分
	while (reader->moveToNextLine())
	{
		// 能进到这里，说明上面的 attr 标签匹配成功了
		// 所以，只需移进缩进匹配的行
		const yuki_line_string* line = reader->getLine();

		if (line == nullptr || line->isBlankLine() || line->getIndent() != commonIndent)
		{
			attrRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
			goto search_finished;
		}
	}
	// 未满足所有条件就跳出
	if (hasAttr)
	{
		attrRegion = reader->cutRegionToCursorFrom(startCursor, commonIndent);
	}

search_finished:
	reader->setCursor(oldCursor);
}
