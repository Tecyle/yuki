#pragma once

/*
	Doctest ���ʶ��Ҫ�㣺

	1. �� ``>>>`` �Ϳո��ͷ������Ϊ�� doctest ��
	2. �ڶ������������ ``...`` �Ϳո��ͷ�ģ���Ϊ�Ǵ��벿�֣�
	   ����ȫ����Ϊ���������
	3. ���������˿��У������ doctest ����
*/
class YukiDoctestNode : public yuki_node
{

};

class YukiDoctest : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();
};