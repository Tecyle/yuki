#pragma once

/*
	������һ���ṹ��Ϊ�˸�֧�� inline ��ǵĽṹ��� inline ����
	��������һ���ṹ
*/
class YukiInlineBlock : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	// �����ṹ��Զƥ��
	virtual bool match() override { return true; }

private:
	bool parseInlineMarkup(YukiNode* parentNode, yuki_cursor& formerCursor);
};