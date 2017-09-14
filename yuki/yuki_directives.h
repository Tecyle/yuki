#pragma once

/*
	directives �ṹ��ɣ�

	simple-reference-name:: <arguments>
		<option list>

		<body>

	ʶ��Ҫ�㣺
	1. ʶ�� simple-reference-name ������� ``::`` �Լ��ո����ʶ��ɹ�
	2. ��һ����������ݣ���ʶ��Ϊ <arguments>������趨������ <arguments>��
	   ��ֱ��ʶ��Ϊ body
	3. ��ʼʶ�� <arguments> ֮��û���������е�ǰ���£��ڶ��е����ݳ���ʶ
	   ��Ϊ <option list>�����ʧ�������ʶ��Ϊ <arguments>�����ҵڶ��е�
	   ������ʼ���㹫������
	4. �������� 3��û���������е�ǰ���£������������С�ڵ��� <arguments>
	   �Ĺ�������������ʶ��Ϊ <option list>���������ʶ��Ϊ <arguments>
	5. ʶ��Ϊ <option list> ֮�󣬺��������� <option list> ���ֵ�����ȫ��
	   ���� <body> ����
	6. ����趨������ <option list>�������֮ǰ�Ĳ���ȫ����Ϊ <arguments>
	   ʶ��
	7. �����һ��û�����ݣ���ڶ��п�ʼ������ʶ�� <option list>�����ʧ�ܣ�
	   ���ҽ��� <arguments>����ʶ��Ϊ <arguments>������ʶ��Ϊ <body>�����
	   �ɹ�����ʶ��Ϊ <option list>������Ϊ <arguments> û�и�����

	���������δ�����ָ����������ݾ���ΪԤ���ı��������
*/

// ָ�������
class YukiDirectives : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();

protected:
	wstring m_directiveName;
};

// ָ����࣬����ָ���������Ϊ ָ������ + ``-directive``������ ``image-directive``
class YukiDirective : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

	virtual bool matchNoBackward();

protected:
	bool m_acceptArguments;
	bool m_acceptOptionLists;
	bool m_acceptBody;
};