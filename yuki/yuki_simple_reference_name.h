#pragma once

/*
	���������ƵĽ�������

	�� ``-._+:`` ��Щ������Ϊ�ָ������ָ����ʿ飬�ָ��������������֣���
	���ܰ��ų��� ``--`` ��������ϡ�
	���ʿ�����ĸ�������Լ����������ַ�(�ݲ�֧��)��ɡ�
	�����ɵ��ʿ��ͷ��

	�������������ڵ㣬����ͨ��ר�к����ƶ�����ָ�벢���ؽ������
*/
class YukiSimpleReferenceName : public yuki_structure
{
public:
	bool parseReferenceName(wstring* res);

protected:
	virtual bool parse(yuki_node*, const yuki_region*) {}
	virtual bool match() {}

	bool parseWordBlock(wstring* res);
};
