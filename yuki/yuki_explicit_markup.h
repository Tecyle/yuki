#pragma once

/*
	��ʽ��ǿ�ʶ��Ҫ�㣺

	1. �� ``..`` ��ͷ���������ٸ�һ���ո���߻س�
	2. ��������� ``..`` ��Ƿ�����������Ϊ��ʽ��ǿ����

	���½ṹ��Ϊ��ʽ��ǿ�ľ���ʵ����
	1. ��ע��
	2. ���ñ�ע��
	3. ������Ŀ�ꡣ
	4. �滻Ŀ�궨�塣
	5. ָ�
	6. ע�͡�

	ʶ�������Ϊ��
	1. ������һ���ַ������Ϊ ``[``���ж��Ƿ�Ϊ��ע�����ñ�ע������Ϊע�͡�
	2. ������һ���ַ������Ϊ ``_``���ж��Ƿ�Ϊ������Ŀ�꣬����Ϊע�͡�
	3. ������һ���ַ������Ϊ ``|``���ж��Ƿ�Ϊ�滻Ŀ�궨�壬����Ϊע�͡�
	4. ������һ���ַ�������ǻ��з����߿��У�ֱ��Ϊע�͡�
	5. �����һ���������� ``::``��ʶ��Ϊָ�����Ϊע�͡�
	6. �������ȫ��Ϊע�͡�

	���������ֻ��һ����ת·�ɣ�����������ڵ㡣
*/
class yuki_explicit_markup : public yuki_structure_parser
{
public:
	yuki_explicit_markup(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_explicit_markup() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;
	virtual bool matchNoBackward();
};