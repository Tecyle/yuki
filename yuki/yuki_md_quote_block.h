#pragma once

#include "yuki_quote_block.h"

/*
	Markdown 风格引用块识别方法：

	1. 如果是 `>` 打头，如果：

		1. 后面是空白字符，则识别成功
		2. 后面是中括号内部带一个 reference name，则识别成功

	2. 从第二行开始，仅识别 `>` 带空格的行，和 Markdown 的语法是不一样的
	3. 最大公共缩进的部分，如果 ``--`` 打头，则识别为 Attribute，并结束
	   当前 quote block，哪怕后面还有 `>` 引导的行

	需要注意虽然这是 Markdown 风格的引用块，但是和 Markdown 不一样：
	
	* 每一行都必须 `>` 打头
	* `>` 后面必须有空格
	* 支持 Attribute
	* 可以指定引用块的类别，类别名称将作为引用块的 class 属性，区分引用块
	  的作用，比如摘要，导读，等
*/
class YukiMdQuoteBlock : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

private:
	bool matchNoBackward();

private:
	wstring m_quoteLevel;
};