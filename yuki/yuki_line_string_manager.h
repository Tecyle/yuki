#pragma once

class yuki_line_string_allocator;
class yuki_line_string;
class yuki_file_string;

/*
	yuki_line_string_manager
	���� yuki_line_string �����ɵ��ͷţ���ʵ�ָ��Ӳ�����

	������ǵ�������ģʽ��
*/
class yuki_line_string_manager
{
public:
	static __inline yuki_line_string_manager* getInstance() { return &m_instance; }

protected:
	yuki_line_string_manager();
	yuki_line_string_manager(const yuki_line_string_manager&) {}
	yuki_line_string_manager& operator=(const yuki_line_string_manager&) {}
	~yuki_line_string_manager();

public:
	/*
		��ȫ�ֵ� yuki_file_string ȫ���ļ�������һ�� line string��
		ln ����ǰ���кţ�
		str ����ǰ���ַ�ָ�룬�ᱻ�ú����޸ĵ���һ�����ס�
	*/
	const yuki_line_string* allocLineStringForFileString(yuki_file_string* fileString,
		int ln, wchar_t* & str);

	/*
		��һ�� region string �й���һ�� line string������������
		�Ķ����ָ�� region �ڲ������֡�
		
		region ָ���˲ü����������У�

			* region ���ָ������������ line string ������������
			  ���ȣ�������γ����ڲ����������ݣ���Ȼ���ټ���������
			  ���������ո񣩡�
			* ָ�����к���������� region �ķ�Χ���򷵻� nullptr��
	*/
	const yuki_line_string* allocLineStringForRegionString(yuki_file_string* fileString,
		const yuki_region* region, int lnInRegion);

	__inline const yuki_line_string* cloneLineString(const yuki_line_string* str);

	__inline void freeLineString(const yuki_line_string* str);

private:
	__inline yuki_line_string* allocLineString(yuki_file_string* fileString);

protected:
	yuki_line_string_allocator* m_allocator;		///< region ������
	static yuki_line_string_manager m_instance;		///< ����ʵ��
};

#define yukiLineStringManager() (yuki_line_string_manager::getInstance())