#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_node.h"
#include "yuki_struct.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_plain_text.h"

/*
	plain text 解析要点：

	1. region 范围内所有文字均解析
	2. 忽略每一行的缩进
	3. 多个空白字符当成一个空格
	4. 转义字符处理
	5. 根据设定决定行尾换行怎样转换
*/
static const wchar_t* g_enterFollowSet = L"abcdefghijklmnopqrstuvwxyz";

bool YukiPlainText::parse(yuki_node* parentNode, const yuki_region* region)
{
	if (region->empty())
		return true;

	yuki_file_reader* reader = getFileReader();
	const yuki_region* oldRegion = reader->selectRegion(region);
	YukiPlainTextNode* textNode = new YukiPlainTextNode;
	wstring& text = textNode->plainText();
	wchar_t lastChar = 0;

	for (; reader->moveToNextChar();)
	{
		wchar_t ch = reader->getChar();

		// 如果是回车符
		if (ch == '\n')
		{
			lastChar = ch;
			continue;
		}

		// 如果是空白字符
		if (isspace(ch))
		{
			// 转义空白，结果应该是什么都不输出
			if (lastChar == '\\')
			{
				lastChar = 0;
				continue;
			}
			// 多个连续空白只添加一个
			if (lastChar != ' ')
			{
				text += ' ';
				lastChar = ' ';
			}
			continue;
		}

		// 如果是转义起始字符
		if (ch == '\\' && lastChar != '\\')
		{
			lastChar = '\\';
			continue;
		}

		// 其它字符，原样输出
		if (lastChar == '\n' && wcschr(g_enterFollowSet, tolower(ch)) != nullptr)
			text += ' ';

		text += ch;
		lastChar = ch;
	}

	parentNode->appendChild(textNode);
	reader->selectRegion(oldRegion);
	return true;
}
