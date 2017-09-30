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
class YukiDirectives : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();

protected:
	wstring m_directiveName;
};

// ָ����࣬����ָ���������Ϊ ָ������ + ``-directive``������ ``image-directive``
class YukiDirective : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

	virtual bool matchNoBackward();

	virtual __inline bool enableInlineMode(bool enable);
	virtual __inline bool inlineMode() const { return m_inlineMode; }

	__inline bool acceptArguments() { return m_acceptArguments; }
	__inline bool acceptOptionLists() { return m_acceptOptionLists; }
	__inline bool acceptBody() { return m_acceptBody; }
	__inline bool acceptInlineMode() { return m_allowInlineMode; }

protected:
	virtual bool matchNoBackwardInlineMode();
	virtual bool matchNoBackwardBlockMode();

	virtual bool parseInlineMode(yuki_node* parentNode, const yuki_region* region);
	virtual bool parseBlockMode(yuki_node* parentNode, const yuki_region* region);

	virtual bool parseArguments(yuki_node* parentNode, const yuki_region* region) = 0;
	virtual bool parseOptionLists(yuki_node* parentNode, const yuki_region* region) = 0;
	virtual bool parseBody(yuki_node* parentNode, const yuki_region* region) = 0;

	virtual yuki_node* getMainDirectiveNode() = 0;

	virtual bool matchArguments();
	virtual bool matchOptionList();
	virtual bool matchBody();

protected:
	const bool m_acceptArguments;
	const bool m_acceptOptionLists;
	const bool m_acceptBody;
	const bool m_allowInlineMode;
	bool m_inlineMode;

	bool m_isFirstLine;

	const yuki_region* m_argumentsRegion;
	yuki_cursor m_argumentsCursor;
	const yuki_region* m_optionListRegion;
	yuki_cursor m_optionListCursor;
	const yuki_region* m_bodyRegion;
	yuki_cursor m_bodyCursor;
};