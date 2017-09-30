#pragma once

/*
	对链接目标进行解析，服务于 Hyperlink Target 结构
	解析规则如下：

	1. 忽略所有空格，不会补任何空格
	2. 可以跨越多行
	3. 转义空白会当成空格
	4. 最后一个字符如果是非转义下划线，就当成是别名链接
	5. 如果解析体为空，则解析失败
*/
class YukiHyperlinkTargetNode;

class YukiLinkUri : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;

protected:
	virtual bool match() override { return false; }
	void parseAliasTarget(YukiHyperlinkTargetNode* node);
	void parseUrlTarget(YukiHyperlinkTargetNode* node);
};