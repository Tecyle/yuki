#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_hyperlink_target.h"
#include "yuki_link_uri.h"

bool YukiLinkUri::parse(YukiNode* parentNode, const yuki_region* region)
{
	YukiFileReader* reader = getFileReader();
	const yuki_region* oldRegion = reader->selectRegion(region);

	YukiHyperlinkTargetNode* node = dynamic_cast<YukiHyperlinkTargetNode*>(parentNode);
	assert(node);

	// 遍历，查找末字符
	yuki_cursor cursor = reader->getCursor();
	bool aliasMode = false;
	wchar_t ch = 0;
	do 
	{
		wchar_t newChar = reader->getChar();
		// 遇到的第一个字符如果是反引号，按照 alias 处理
		if (ch == 0 && newChar == '`')
		{
			aliasMode = true;
			break;
		}
		// 上一个字符是斜杠，那么标记这个字符是转义字符
		newChar = ch == '\\' ? 'y' : reader->getChar();
		if (isspace(newChar))
			continue;

		if (ch == 0)
		{
			cursor = reader->getCursor();
		}
		ch = newChar;
		aliasMode = ch =='_';
	} while (reader->moveToNextChar());

	if (ch == 0)
	{
		// 如果没有遇到任何非空字符
		reader->selectRegion(oldRegion);
		return false;
	}
	// 解析 uri，注意，不去检查 uri 的正确性
	reader->setCursor(cursor);
	if (aliasMode)
		parseAliasTarget(node);
	else
		parseUrlTarget(node);

	reader->selectRegion(oldRegion);
	return true;
}

void YukiLinkUri::parseAliasTarget(YukiHyperlinkTargetNode* node)
{
	YukiFileReader* reader = getFileReader();
	node->setTargetType(HyperlinkTarget_alias);
	// 第一个字符为有效字符，判断是否是 反引号，从而决定解析类型及错误
	bool hasQuote = reader->getChar() == '`';
	if (hasQuote)
		reader->moveToNextChar();
	
	wstring& target = node->uri();
	bool isLastCharNotWord = false;
	do 
	{
		wchar_t ch = reader->getChar();
		
		if (ch == '\\')
		{
			// 转义字符，原样保留
			reader->moveToNextChar();
			ch = reader->getChar();
			target += ch;
			isLastCharNotWord = !yuki_isWordChar(ch);
			continue;
		}

		if (yuki_isWordChar(ch))
		{
			// 字符数字直接附加
			target += ch;
			isLastCharNotWord = false;
			continue;
		}

		// 如果遇到反引号
		if (hasQuote)
		{
			if (ch == '`')
			{
				reader->moveToNextChar();
				break;
			}
		}
		else if (ch == '_')
			break;

		// 其它字符，均翻译成简单的横线
		if (!isLastCharNotWord)
		{
			target += '-';
			isLastCharNotWord = true;
		}
	} while (reader->moveToNextChar());
}

void YukiLinkUri::parseUrlTarget(YukiHyperlinkTargetNode* node)
{
	YukiFileReader* reader = getFileReader();
	node->setTargetType(HyperlinkTarget_external);
	// 空白字符忽略，转义字符原样输出
	wstring& target = node->uri();
	do 
	{
		wchar_t ch = reader->getChar();

		if (isspace(ch))
			continue;

		if (ch == '\\')
		{
			reader->moveToNextChar();
			ch = reader->getChar();
		}

		target += ch;
	} while (reader->moveToNextChar());
}
