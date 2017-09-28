#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_region_manager.h"
#include "yuki_file_string.h"
#include "yuki_object_allocator.h"

typedef yuki_object_allocator<yuki_region,
	yuki_region_type, const yuki_cursor&, const yuki_cursor&, int>
	yuki_region_allocator;

yuki_region_manager yuki_region_manager::m_instance;

//////////////////////////////////////////////////////////////////////////
// yuki_region_manager
yuki_region_manager::yuki_region_manager()
	: m_allocator(new yuki_region_allocator())
{
}

yuki_region_manager::~yuki_region_manager()
{
	delete m_allocator;
}

const yuki_region* yuki_region_manager::allocRegion(yuki_region_type type, 
	const yuki_cursor& startPos, const yuki_cursor& endPos, int indent)
{
	return m_allocator->allocObject(type, startPos, endPos, indent);
}

const yuki_region* yuki_region_manager::cloneRegion(const yuki_region* region)
{
	return m_allocator->cloneObject(*region);
}

void yuki_region_manager::freeRegion(const yuki_region* region)
{
	return m_allocator->freeObject(const_cast<yuki_region*>(region));
}

/*
	区域裁剪算法：
	1. 在一个行连续区域内裁剪一个行连续区域，所得的区域还是行连续区域，起点游标和终点游标
	   均加上原区域的起点游标，缩进直接相加；
	2. 在一个行连续区域内裁剪一个块区域，所得的区域是块区域，起点游标和终点游标均加上原区
	   域的起点游标，缩进直接相加，边界即为起点终点游标；
	3. 在一个块区域中裁剪一个行连续区域，所得的区域是块区域，起点游标和终点游标均加上原区
	   域的起点游标，缩进直接相加，边界保留原区域边界；
	4. 在一个块区域中裁剪一个块区域，所得的区域还是块区域，起点游标和终点游标均加上原区域
	   的起点游标，缩进直接相加，边界即为目标区域的起点终点游标。

	此外，如果相对区域的终点游标是无效游标，则终点游标需要替换成原区域的终点游标。
*/
const yuki_region* yuki_region_manager::allocFromSubRegion(yuki_file_string* buffer, 
	const yuki_region* originalRegion, const yuki_cursor& startPos, const yuki_cursor& endPos, 
	int indent, yuki_region_type type /*= Yuki_linedRegion*/)
{
	if (originalRegion->getRegionType() != Yuki_linedRegion || type == Yuki_linedRegion)
		type = Yuki_blockRegion;

	yuki_region* region = m_allocator->allocObject(type, startPos, endPos, indent);

	const yuki_cursor& originalStartCursor = originalRegion->begin();
	if (!region->endCursor.isValid())
		region->endCursor = originalRegion->end();
	region->indent += originalRegion->getIndent();

	if (type == Yuki_blockRegion)
	{
		region->leftEdgeCursor = startPos;
		region->rightEdgeCursor = endPos;
	}
	else if (originalRegion->getRegionType() == Yuki_blockRegion)
	{
		region->leftEdgeCursor = originalRegion->leftEdgeCursor;
		region->rightEdgeCursor = originalRegion->rightEdgeCursor;
	}

	return region;
}
