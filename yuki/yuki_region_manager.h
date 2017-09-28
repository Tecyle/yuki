#pragma once

class yuki_region_allocator;
class yuki_file_string;

/*
	yuki_region_manager
	负责 yuki_region 的生成的释放，并实现附加操作。

	这个类是单例工作模式。
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
		指定起终点以及切割方式，从 originalRegion 中切割一个采用绝对坐标的
		子区域并返回。

		startPos 和 endPos 都采用绝对坐标。
	*/
	const yuki_region* allocFromSubRegion(yuki_file_string* buffer, const yuki_region* originalRegion, 
		const yuki_cursor& startPos, const yuki_cursor& endPos, int indent, 
		yuki_region_type type = Yuki_linedRegion);

protected:
	yuki_region_allocator* m_allocator;			///< region 分配器
	static yuki_region_manager m_instance;		///< 单例实例
};

#define yukiRegionManager() (yuki_region_manager::getInstance())