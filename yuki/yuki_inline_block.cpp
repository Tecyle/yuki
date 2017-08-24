#include "stdafx.h"
#include "yuki.h"
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
bool YukiInlineBlock::parse(YukiStruct* parent)
{

}
