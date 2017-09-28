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
	����ü��㷨��
	1. ��һ�������������ڲü�һ���������������õ���������������������α���յ��α�
	   ������ԭ���������α꣬����ֱ����ӣ�
	2. ��һ�������������ڲü�һ�����������õ������ǿ���������α���յ��α������ԭ��
	   �������α꣬����ֱ����ӣ��߽缴Ϊ����յ��αꣻ
	3. ��һ���������вü�һ���������������õ������ǿ���������α���յ��α������ԭ��
	   �������α꣬����ֱ����ӣ��߽籣��ԭ����߽磻
	4. ��һ���������вü�һ�����������õ������ǿ���������α���յ��α������ԭ����
	   ������α꣬����ֱ����ӣ��߽缴ΪĿ�����������յ��αꡣ

	���⣬������������յ��α�����Ч�α꣬���յ��α���Ҫ�滻��ԭ������յ��αꡣ
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
