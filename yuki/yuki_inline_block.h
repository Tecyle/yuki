#pragma once

/*
	本身不是一个结构，为了给支持 inline 标记的结构添加 inline 解析
	而创建的一个结构
*/
class YukiInlineBlock : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	// 内联结构永远匹配
	virtual bool match() override { return true; }

private:
	bool parseInlineMarkup(yuki_node* parentNode, yuki_cursor& formerCursor);
};