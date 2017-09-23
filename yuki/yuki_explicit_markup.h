#pragma once

/*
	显式标记块识别要点：

	1. 以 ``..`` 打头，后面至少跟一个空格或者回车
	2. 遇到相对于 ``..`` 标记非缩进的行认为显式标记块结束

	以下结构均为显式标记块的具体实例：
	1. 脚注。
	2. 引用标注。
	3. 超链接目标。
	4. 替换目标定义。
	5. 指令。
	6. 注释。

	识别的依据为：
	1. 遇到第一个字符，如果为 ``[``，判断是否为脚注和引用标注，否则为注释。
	2. 遇到第一个字符，如果为 ``_``，判断是否为超链接目标，否则为注释。
	3. 遇到第一个字符，如果为 ``|``，判断是否为替换目标定义，否则为注释。
	4. 遇到第一个字符，如果是换行符或者空行，直接为注释。
	5. 如果第一行内遇到了 ``::``，识别为指令，否则为注释。
	6. 其它情况全都为注释。

	这个解析器只是一个中转路由，本身不会产生节点。
*/
class YukiExplicitMarkup : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override;
	virtual bool matchNoBackward();
};