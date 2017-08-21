#include "stdafx.h"
#include "yuki.h"
#include "yuki_inline_markup.h"

bool YukiInlineMarkup::parse(YukiStruct* parent)
{
	m_fileLoader->saveCursor();

	if (!matchPrefix())
		goto match_failed;

	if (!searchInfix())
		goto match_failed;

	if (!searchSufix())
		goto match_failed;

	return true;
match_failed:
	m_fileLoader->restoreCursor();
	return false;
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
static bool matchPrefixRules(wchar_t prev, wchar_t next)
{
	const static wchar_t* rule6TokenSet = L"-:/'\"<([{：“‘《（【";
	const static wchar_t* rule5StartSet = L"'\"<([{‘“《（【";
	const static wchar_t* rule5EndSet = L"'\">)]}’”》）】";

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
	const static wchar_t* rule7Set = L"-.,:;!?\\/'\")]}>。，：；！？、’”）】》";

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
	return matchPrefixRules(prev, next) && matchSufixRules(prev, next, allowEscape);
}

bool YukiInlineMarkup::matchPrefix()
{
	wchar_t prevChar = m_fileLoader->peekPreviousCharInline();

	assert(m_prefix != nullptr);
	if (!m_fileLoader->match(m_prefix))
		return false;

	wchar_t curChar = m_fileLoader->getChar();

	return matchPrefixRules(prevChar, curChar);
}

bool YukiInlineMarkup::searchInfix()
{
	// 不存在中缀的话，直接成功
	if (m_infix == nullptr)
		return true;

	return searchAndMatch(m_infix, &m_text1);
}

bool YukiInlineMarkup::searchSufix()
{
	return searchAndMatch(m_sufix, m_infix == nullptr ? &m_text1 : &m_text2);
}

bool YukiInlineMarkup::searchAndMatch(const wchar_t* matchStr, YukiString* readText)
{
	bool matchSucc = false;

	assert(matchStr != nullptr);

	wchar_t* textOffset = m_fileLoader->getCStr();
	int textLength = 0;
	wchar_t curChar = m_fileLoader->getChar();
	while (!outOfRegion())
	{
		if (!m_fileLoader->match(matchStr))
		{
			++textLength;
			m_fileLoader->moveToNextChar();
			curChar = m_fileLoader->getChar();
		}
		else
		{
			// 检查规则 3
			if (textLength == 0)
				return false;

			if (matchStr == m_infix)
				matchSucc = matchInfixRules(curChar, m_fileLoader->getChar(), m_allowEscapeNearMark);
			else
				matchSucc = matchSufixRules(curChar, m_fileLoader->getChar(), m_allowEscapeNearMark);

			if (matchSucc)
				break;

			// 匹配失败，继续尝试向后搜索
			textLength += wcslen(matchStr);
			m_fileLoader->moveToNextChar(wcslen(matchStr));
			curChar = m_fileLoader->getChar();
		}
	}

	if (!matchSucc)
		return false;

	readText->init(textOffset, textLength);
	return true;
}
