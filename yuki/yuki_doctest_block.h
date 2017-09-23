#pragma once

/*
	Doctest ���ʶ��Ҫ�㣺

	1. �� ``>>>`` �Ϳո��ͷ������Ϊ�� doctest ��
	2. �ڶ������������ ``...`` �Ϳո��ͷ�ģ���Ϊ�Ǵ��벿�֣�
	   ����ȫ����Ϊ���������
	3. ���������˿��У������ doctest ����
*/
class YukiDoctestNode : public YukiNode
{

};

class YukiDoctest : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();
};