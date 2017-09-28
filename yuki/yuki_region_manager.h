#pragma once

class yuki_region_allocator;
class yuki_file_string;

/*
	yuki_region_manager
	���� yuki_region �����ɵ��ͷţ���ʵ�ָ��Ӳ�����

	������ǵ�������ģʽ��
*/
class yuki_region_manager
{
public:
	static __inline yuki_region_manager* getInstance() { return &m_instance; }

protected:
	yuki_region_manager();
	yuki_region_manager(const yuki_region_manager&) {}
	yuki_region_manager& operator=(const yuki_region_manager&) {}
	~yuki_region_manager();

public:
	__inline const yuki_region* allocRegion(yuki_region_type type, const yuki_cursor& startPos,
		const yuki_cursor& endPos, int indent);

	__inline const yuki_region* cloneRegion(const yuki_region* region);

	__inline void freeRegion(const yuki_region* region);

	/*
		ָ�����յ��Լ��иʽ���� originalRegion ���и�һ�����þ��������
		�����򲢷��ء�

		startPos �� endPos �����þ������ꡣ
	*/
	const yuki_region* allocFromSubRegion(yuki_file_string* buffer, const yuki_region* originalRegion, 
		const yuki_cursor& startPos, const yuki_cursor& endPos, int indent, 
		yuki_region_type type = Yuki_linedRegion);

protected:
	yuki_region_allocator* m_allocator;			///< region ������
	static yuki_region_manager m_instance;		///< ����ʵ��
};

#define yukiRegionManager() (yuki_region_manager::getInstance())