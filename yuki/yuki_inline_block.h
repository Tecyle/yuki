#pragma once

/*
	������һ���ṹ��Ϊ�˸�֧�� inline ��ǵĽṹ��� inline ����
	��������һ���ṹ
*/
class YukiInlineBlock : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	// �����ṹ��Զƥ��
	virtual bool match() override { return true; }

private:
	bool parseInlineMarkup(yuki_node* parentNode, yuki_cursor& formerCursor);
};