#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_inline_markup.h"

bool YukiInlineMarkup::match()
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	yuki_cursor startCursor;
	yuki_cursor endCursor;
	bool succ = false;

	m_textRegion1 = nullptr;
	m_textRegion2 = nullptr;

	// 匹配前缀
	if (!matchPrefix())
		goto match_finished;

	startCursor = reader->getCursor();
	// 如果存在中缀，匹配中缀
	if (m_infix != nullptr)
	{
		if (!matchInfix(endCursor))
			goto match_finished;

		m_textRegion1 = reader->cutRegionBetween(startCursor, endCursor);
		startCursor = reader->getCursor();
	}

	// 匹配后缀
	if (!matchSuffix(endCursor))
		goto match_finished;

	if (m_infix == nullptr)
		m_textRegion1 = reader->cutRegionBetween(startCursor, endCursor);
	else
		m_textRegion2 = reader->cutRegionBetween(startCursor, endCursor);

	// 记录匹配成功之后的位置，防止重复搜索
	m_endCursor = reader->getCursor();
	succ = true;

match_finished:
	reader->setCursor(oldCursor);
	return succ;
}

/*
	前缀识别原则：

	1. 内联标记的开始前缀后面紧跟着的字符必须不能是空白字符。
	4. 内联标记的开始前缀和结束后缀之前都不能紧跟着一个未转义的反斜杠(有
	   一个特殊情况，那就是在内联文本中，会忽略所有反斜杠的转义含义，所以
	   在内联文本的结束后缀之前是可以出现反斜杠的)。
	5. 如果一个开始前缀之前紧跟着一个开始符号(ASCII 字符集中的 ``' " < ( [ {``
	   或者是非 ASCII 字符集中的类似的符号)，那么这个开始前缀的后面就不能紧跟着
	   对应的关闭符号 (ASCII 字符集中的 ' " > ) ] } 或者是非 ASCII 字符集中的类
	   似的符号)。目前，yuki 仅支持 ASCII 的开始符号和关闭符号。
	6. 如果没有开启 simple-inline-markup 选项的话，那么，内联标记的开始前缀必须
	   是一个文本块的最开始，或者是紧跟在以下符号后面：
			
			* 空白字符
			* 下列标点符号：- : / ' " < ( [ { 或者其它类似的非 ASCII 字符集的标点符号
*/
const static wchar_t* rule6TokenSet = L"-:/'\"<([{：“‘《（【";
const static wchar_t* rule5StartSet = L"'\"<([{‘“《（【";
const static wchar_t* rule5EndSet = L"'\">)]}’”》）】";
const static wchar_t* rule7Set = L"-.,:;!?\\/'\")]}>。，：；！？、’”）】》";
static bool matchPrefixRules(wchar_t prev, wchar_t next)
{

	// 规则 6
	if (!yuki_simple_inline_markup())
	{
		if (prev != 0									// 不是一行的最开始位置
			&& prev != 1								// 没有跟在转义字符后面
			&& !isspace(prev)							// 不是空白字符
			&& wcschr(rule6TokenSet, prev) == nullptr	// 没有紧跟在允许的开始符号后面
			)
			return false;
	}

	// 规则 4 解析过程可以保证，前缀符号如果有转义会被优先转义
	if (prev == '\\')
		return false;

	// 规则 1
	if (isspace(next))
		return false;

	// 规则 5
	if (wcschr(rule5StartSet, prev) != nullptr
		&& wcschr(rule5EndSet, next) != nullptr)
		return false;

	return true;
}

bool YukiInlineMarkup::matchPrefix()
{
	yuki_file_reader* reader = getFileReader();
	wchar_t prevChar = reader->peekPreviousChar();

	assert(m_prefix != nullptr);
	if (!reader->matchStr(m_prefix))
		return false;

	wchar_t curChar = reader->getChar();

	return matchPrefixRules(prevChar, curChar);
}

bool YukiInlineMarkup::matchInfix(yuki_cursor& cursor)
{
	assert(m_infix != nullptr);
	return searchAndMatch(m_infix, cursor);
}

bool YukiInlineMarkup::matchSuffix(yuki_cursor& cursor)
{
	return searchAndMatch(m_suffix, cursor);
}

/*
	后缀识别原则：

	2. 内联标记的结束后缀前面紧跟着的字符必须不能是空白字符。
	3. 内联标记的开始前缀和结束后缀之间必须间隔至少一个字符，换句话说就是
	   内联标记所作用的内容不能是空的。
	4. 内联标记的开始前缀和结束后缀之前都不能紧跟着一个未转义的反斜杠(有
	   一个特殊情况，那就是在内联文本中，会忽略所有反斜杠的转义含义，所以
	   在内联文本的结束后缀之前是可以出现反斜杠的)。
	7. 如果没有开启 simple-inline-markup 选项的话，那么，内联标记的结束后缀必须
	   是一个文本块的结尾，或者后面紧跟着以下符号：
	
			* 空白字符
			* 下列标点符号：- . , : ; ! ? \ / ' " ) ] } > 或者其它类似的非 ASCII 字符集的标点符号
*/
static bool matchSufixRules(wchar_t prev, wchar_t next, bool allowEscape)
{
	// 检查规则 2
	if (isspace(next))
		return false;

	// 检查规则 4
	if (!allowEscape && next == '\\')
		return false;

	// 检查规则 7
	if (!yuki_simple_inline_markup())
	{
		if (!isspace(next) && wcschr(rule7Set, next) == nullptr)
			return false;
	}

	return true;
}

/*
	中缀匹配，需要同时满足前缀和后缀的条件
*/
inline static bool matchInfixRules(wchar_t prev, wchar_t next, bool allowEscape)
{
	// 规则 6
	if (!yuki_simple_inline_markup())
	{
		if (prev != 0									// 不是一行的最开始位置
			&& !isspace(prev)							// 不是空白字符
			&& wcschr(rule6TokenSet, prev) == nullptr	// 没有紧跟在允许的开始符号后面
			)
			return false;
	}

	return matchPrefixRules(prev, next) && matchSufixRules(prev, next, allowEscape);
}

bool YukiInlineMarkup::searchAndMatch(const wchar_t* pattern, yuki_cursor& contentCursor)
{
	yuki_file_reader* reader = getFileReader();
	bool matchSucc = false;

	assert(pattern != nullptr);

	int textLength = 0;
	wchar_t lastChar = 0;
	yuki_cursor lastCursor;
	for(;reader->moveToNextChar();)
	{
		lastCursor = reader->getCursor();
		if (!reader->matchStr(pattern))
		{
			textLength++;
			lastChar = reader->getChar();
			continue;
		}
		// 遇到匹配的模式
		// 检查规则 3
		if (textLength == 0)
			return false;

		if (pattern == m_infix)
			matchSucc = matchInfixRules(lastChar, reader->getChar(), m_allowEscapeNearMark);
		else
			matchSucc = matchSufixRules(lastChar, reader->getChar(), m_allowEscapeNearMark);

		if (matchSucc)
			break;

		// 匹配失败，继续尝试向后搜索
		textLength += wcslen(pattern);
		lastChar = reader->getChar();
	}

	if (!matchSucc)
		return false;

	contentCursor = lastCursor;
	return true;
}
