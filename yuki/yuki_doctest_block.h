#pragma once

/*
	Doctest ���ʶ��Ҫ�㣺

	1. �� ``>>>`` �Ϳո��ͷ������Ϊ�� doctest ��
	2. �ڶ������������ ``...`` �Ϳո��ͷ�ģ���Ϊ�Ǵ��벿�֣�
	   ����ȫ����Ϊ���������
	3. ���������˿��У������ doctest ����
*/
class yuki_doctest_block : public yuki_structure_parser
{
public:
	yuki_doctest_block(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_doctest_block() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();
};