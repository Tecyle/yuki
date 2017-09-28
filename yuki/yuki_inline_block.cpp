#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_matcher.h"
#include "yuki_plain_text.h"
#include "yuki_inline_block.h"

/*
	内联结构解析要点：

	一共存在以下这些内联标记：

	1. 格式标记
	
		* ``code``
		* *emphasis*
		* **strong**
		* ```key```
		* --deleted--
		* __underline__
		* ==mark==
		* ***bold and italic***
		* x ^^2^^
		* H vv2vv O

	2. 结构标记

		* ![role](text)
		* ![role][reference]
		* <abbr>(abbreviation)
		* |substitution|
		* |substitution link|_
		* _`inline internal target`
		* `hyper link reference`_  word_  `link <inline>`_  anonymous__
		* [foot note]_
		* `preintepreted`  :role:`text`  `text`:role:
		* [markdown hyper link](somewhere)
		* standalone link target

	识别的原则：

	1. 内联标记的开始前缀后面紧跟着的字符必须不能是空白字符。
	2. 内联标记的结束后缀前面紧跟着的字符必须不能是空白字符。
	3. 内联标记的开始前缀和结束后缀之间必须间隔至少一个字符，换句话说就是
	   内联标记所作用的内容不能是空的。
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
	
	7. 如果没有开启 simple-inline-markup 选项的话，那么，内联标记的结束后缀必须
	   是一个文本块的结尾，或者后面紧跟着以下符号：
	
			* 空白字符
			* 下列标点符号：- . , : ; ! ? \ / ' " ) ] } > 或者其它类似的非 ASCII 字符集的标点符号

	有以下符号被作为起始符号：
	
	* ` - _ = ^ v ! < | [ :

	识别的顺序：

	1. 
*/
static const wchar_t* g_startCharSet = L"*`-_=^v!<|[";
static const wchar_t* g_headSet = L"-:/'\"<([{：、‘“《（【";
static const wchar_t* g_fllowSet = L"-.,:;!?\\/'\")]}。，：；！？、’”）】";

bool YukiInlineBlock::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor startCursor = reader->getCursor();
	wchar_t ch;
	bool isLastCharInHeadSet = true;
	bool isLastCharSlash = false;
	const yuki_region* oldRegion = reader->selectRegion(region);

	for(;;)
	{
		ch = reader->getChar();
		// 判断当前是否可以进入内联标记检测
		if ((yuki_simple_inline_markup() || isLastCharInHeadSet)
			&& !isLastCharSlash
			&& wcschr(g_startCharSet, ch) != nullptr)
		{
			if (parseInlineMarkup(parentNode, startCursor))
			{
				continue;
			}
		}
		// 判断该字符后面是否可以跟着内联标记
		if (!yuki_simple_inline_markup())
		{
			isLastCharInHeadSet = (isspace(ch) || wcschr(g_headSet, ch));
		}
		isLastCharSlash = ch == '\\';
		
		if(!reader->moveToNextChar())
			break;
	}

	// 最后一段 plain_text 生成
	getParser(L"plain_text")->parse(parentNode, reader->cutRegionFromCursorToEnd());
	reader->selectRegion(oldRegion);
	return true;
}

bool YukiInlineBlock::parseInlineMarkup(YukiNode* parentNode, yuki_cursor& formerCursor)
{
	yuki_file_reader* reader = getFileReader();
	wchar_t ch = reader->getChar();
	yuki_cursor cursor = reader->getCursor();

	for (auto matcherName : *getMatcherCollection()->getInlineFollowSet(ch))
	{
		if (getParser(matcherName)->parse(parentNode, reader->getRegion()))
		{
			YukiPlainText* plainText = dynamic_cast<YukiPlainText*>(getParser(L"plain_text"));
			plainText->parseInPenult(parentNode, reader->cutRegionBetween(formerCursor, cursor));
			formerCursor = reader->getCursor();
			return true;
		}
	}

	return false;
}
