#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_footnote_citation_definition.h"
#include "yuki_hyperlink_target.h"

bool YukiHyperlinkTarget::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);

	if (!matchNoBackward())
	{
		reader->setCursor(oldCursor);
		return false;
	}

	YukiHyperlinkTargetNode* node = new YukiHyperlinkTargetNode;

	if (!getParser(L"link_uri")->parse(node, reader->cutRegionFromCursorToEnd()))
	{
		node->setTargetType(HyperlinkTarget_internal);
	}

	return true;
}

bool YukiHyperlinkTarget::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

/*
	识别超链接目标的逻辑：

	1. ``_`` 打头
	2. 下划线后面如果没有反引号的话
		1. 下划线后面不能是空白字符
		2. 遇到冒号结束
		3. 冒号前面不能是空白字符
		4. 下划线和冒号之间至少有一个字符
	3. 下划线后面如果是反引号的话
		1. 遇到 `: 结束
		2. 反引号前后不能有空格
		3. 反引号内部至少有一个字符
	4. 冒号后面必须有空格
	5. 在识别的时候非字母数字的地方会统一被替换成单一的减号
*/
bool YukiHyperlinkTarget::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();

	// 先识别匿名链接目标
	if (reader->matchStr(L"__:"))
	{
		if (!reader->cursorAtLineEnd() && reader->skipSpaces() == 0)
			return false;

		m_anonymousName = true;
		return true;
	}

	// 后面的模式均不是匿名的
	m_anonymousName = false;
	if (!reader->matchChar('_'))
		return false;
	// 匹配链接名称
	if (reader->matchChar('`'))
	{		
		if (!readTargetName('`'))
			return false;

		if (!reader->matchChar(':'))
			return false;
	}
	else
	{
		if (!readTargetName(':'))
			return false;
	}
	// 匹配后续空格
	if (!reader->cursorAtLineEnd() && reader->skipSpaces() == 0)
		return false;

	return true;
}

bool YukiHyperlinkTarget::readTargetName(wchar_t endChar)
{
	yuki_file_reader* reader = getFileReader();
	wchar_t ch = reader->getChar();
	bool lastCharIsWordChar = true;
	bool lastCharIsSpace = false;
	m_matchedName = L"";

	if (isspace(ch) || ch == endChar)
		return false;

	do
	{
		ch = reader->getChar();
		if (ch == 0)
			return false;
		// 判断是否遇到结束符号
		if (ch == endChar)
		{
			// 结束符之前不能有非转义的空格
			if (lastCharIsSpace)
				return false;

			if (m_matchedName.size() > 0)
			{
				// 移除末尾的 - 符号
				if (m_matchedName.back() == '-')
					m_matchedName.pop_back();
			}
			reader->moveToNextChar(false);
			return true;
		}
		// 处理转义字符
		if (ch == '\\')
		{
			reader->moveToNextChar(false);
			ch = reader->getChar();
			if (isspace(ch))
				continue;
		}
		// 遇到字母数字，直接移进
		if (yuki_isWordChar(ch))
		{
			lastCharIsWordChar = true;
			m_matchedName += ch;
		}
		else if (lastCharIsWordChar)
		{
			// 遇到其它意外的符号，全部转换成单个的 ``-``
			lastCharIsWordChar = false;
			m_matchedName += '-';
		}

		lastCharIsSpace = isspace(ch);
	} while (reader->moveToNextChar(false));

	return false;
}
