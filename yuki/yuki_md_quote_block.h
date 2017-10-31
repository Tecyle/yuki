#pragma once

#include "yuki_quote_block.h"

/*
	Markdown ������ÿ�ʶ�𷽷���

	1. ����� `>` ��ͷ�������

		1. �����ǿհ��ַ�����ʶ��ɹ�
		2. �������������ڲ���һ�� reference name����ʶ��ɹ�

	2. �ӵڶ��п�ʼ����ʶ�� `>` ���ո���У��� Markdown ���﷨�ǲ�һ����
	3. ��󹫹������Ĳ��֣���� ``--`` ��ͷ����ʶ��Ϊ Attribute��������
	   ��ǰ quote block�����º��滹�� `>` ��������

	��Ҫע����Ȼ���� Markdown �������ÿ飬���Ǻ� Markdown ��һ����
	
	* ÿһ�ж����� `>` ��ͷ
	* `>` ��������пո�
	* ֧�� Attribute
	* ����ָ�����ÿ�����������ƽ���Ϊ���ÿ�� class ���ԣ��������ÿ�
	  �����ã�����ժҪ����������
*/
class yuki_md_quote_block : public yuki_structure_parser
{
public:
	yuki_md_quote_block(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_md_quote_block() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

private:
	bool matchNoBackward();

private:
	wstring m_quoteLevel;
};