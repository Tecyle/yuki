#pragma once

/*
	����һ�� yuki �ĵ��Ľ�����
*/

class yuki_structure_parser
{
public:
	yuki_structure_parser(yuki_session* session);
	virtual ~yuki_structure_parser();

public:
	/*
		������������ȥ�����ýṹ����������ɹ������� true�����򷵻� false��
		��������ɹ����α���ƶ�������ṹ����һ��λ�ã�
		�������ʧ�ܣ��α꽫�����ƶ���

		parentNode
			�����˵�ǰ�����ṹ�ĸ��ڵ㣬��ǰ�����õ��Ľڵ㽫��׷��������ڵ�
			��������һ���ӽڵ�ĺ��档

		region
			�����������޶��˽����ķ�Χ�� region ��ָ��ķ�Χ�ڽ��С�
	*/
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) = 0;
	
	/*
		ƥ�亯����Ŀ��������̽��ǰλ���Ƿ��ܳɹ������ýṹ��������Խ�������
		���� true�����򷵻� false��

		�������������� false����ô���� parse ���ص�ҲӦ���� false��

		���ܽ����ɹ�����α궼����ı�ԭ����λ�á�

		��ƥ���ʱ�򣬽��ڵ�ǰ�������������
	*/
	virtual bool match() = 0;

public:
	__inline const wchar_t* getName() const { return m_name; }
	__inline yuki_structure_type getStructureType() const { return m_type; }

protected:
	/*
		�������������ݽṹ����������ȡ��Ӧ�Ľ�������
	*/
	__inline yuki_structure_parser* getParser(const wchar_t* name);
	
	/*
		������������õ�ǰ�Ự�� reader��
	*/
	__inline yuki_file_reader* getFileReader();

protected:
	const wchar_t* m_name;				///< ��ǰ�ṹ����������
	yuki_structure_type m_type;			///< ��ǰ�ṹ������������
	yuki_session* m_session;			///< ���浱ǰ�Ự
};
