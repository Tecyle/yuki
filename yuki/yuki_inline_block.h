#pragma once

/*
	������һ���ṹ��Ϊ�˸�֧�� inline ��ǵĽṹ��� inline ����
	��������һ���ṹ����λ������ body
*/
class yuki_inline_block : public yuki_structure_parser
{
public:
	yuki_inline_block(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_inline_block() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	// �����ṹ��Զƥ��
	virtual bool match() override { return true; }

private:
	bool parseInlineMarkup(yuki_node* parentNode, yuki_cursor& formerCursor);
};