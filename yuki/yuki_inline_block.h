#pragma once

/*
	������һ���ṹ��Ϊ�˸�֧�� inline ��ǵĽṹ��� inline ����
	��������һ���ṹ
*/
class YukiInlineBlock : public YukiStruct
{
public:
	virtual void parse(YukiStruct* parent);
};