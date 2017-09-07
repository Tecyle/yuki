#pragma once

/*
	简单引用名称的解析规则：

	由 ``-._+:`` 这些符号作为分隔符，分隔单词块，分隔符不能连续出现，即
	不能挨着出现 ``--`` 这样的组合。
	单词块由字母，数字以及中文文字字符(暂不支持)组成。
	必须由单词块打头。

	本解析不产生节点，而是通过专有函数移动解析指针并返回解析结果
*/
class YukiSimpleReferenceName : public YukiStruct
{
public:
	bool parseReferenceName(wstring* res);

protected:
	virtual bool parse(YukiNode*, const YukiRegion*) {}
	virtual bool match() {}

	bool parseWordBlock(wstring* res);
};
