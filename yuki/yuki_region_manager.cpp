#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_region_manager.h"
#include "yuki_file_string.h"

yuki_region_manager yuki_region_manager::m_instance;

//////////////////////////////////////////////////////////////////////////
// yuki_region_allocator
class yuki_region_allocator
{
public:
	yuki_region_allocator();
	~yuki_region_allocator();

public:
	__inline yuki_region* allocRegion(yuki_region_type type, const yuki_cursor& startPos,
		const yuki_cursor& endPos, int indent);

	__inline yuki_region* cloneRegion(yuki_region* region);

	__inline void freeRegion(yuki_region* region);

protected:
	void extendPoolIfNeeded();
	__inline yuki_region* getValidRegion();

protected:
	typedef vector<yuki_region*> yuki_region_pool;
	yuki_region_pool m_regionPool;
	yuki_region_pool m_releasedRegionPool;
	int m_usedPosition;
	static const int m_poolSize = 1024;
};

yuki_region_allocator::yuki_region_allocator()
{
	m_usedPosition = 0;
}

yuki_region_allocator::~yuki_region_allocator()
{
	for (auto region : m_regionPool)
	{
		free(region);
	}
}

yuki_region* yuki_region_allocator::allocRegion(yuki_region_type type, const yuki_cursor& startPos, const yuki_cursor& endPos, int indent)
{
	return new(getValidRegion()) yuki_region(type, startPos, endPos, indent);
}

yuki_region* yuki_region_allocator::cloneRegion(yuki_region* region)
{
	return new(getValidRegion()) yuki_region(region);
}

void yuki_region_allocator::freeRegion(yuki_region* region)
{
	m_releasedRegionPool.push_back(region);
}

void yuki_region_allocator::extendPoolIfNeeded()
{
	if (!m_releasedRegionPool.empty())
		return;

	if (m_usedPosition < m_poolSize)
		return;

	yuki_region* regions = (yuki_region*)malloc(sizeof(yuki_region) * m_poolSize);
	m_regionPool.push_back(regions);
	m_usedPosition = 0;
}

yuki_region* yuki_region_allocator::getValidRegion()
{
	extendPoolIfNeeded();

	yuki_region* region = nullptr;
	// �Ȳ����ͷų������Ƿ��п��õģ�û�п��õľ���չһ��
	if (m_releasedRegionPool.empty())
	{
		region = m_regionPool[m_usedPosition++];
	}
	else
	{
		region = m_releasedRegionPool.back();
		m_releasedRegionPool.pop_back();
	}

	assert(region);
	return region;
}

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
	return m_allocator->allocRegion(type, startPos, endPos, indent);
}

const yuki_region* yuki_region_manager::cloneRegion(const yuki_region* region)
{
	return m_allocator->cloneRegion(const_cast<yuki_region*>(region));
}

void yuki_region_manager::freeRegion(const yuki_region* region)
{
	return m_allocator->freeRegion(const_cast<yuki_region*>(region));
}

/*
	����ü��㷨��
	1. ��һ�������������ڲü�һ���������������õ���������������������α���յ��α�
	   ������ԭ���������α꣬����ֱ����ӣ�
	2. ��һ�������������ڲü�һ�����������õ������ǿ���������α���յ��α������ԭ��
	   �������α꣬����ֱ����ӣ�
	3. ��һ���������вü�һ���������������õ������ǿ���������α���յ��α������ԭ��
	   �������α꣬����ֱ����ӣ�
	4. ��һ���������вü�һ�����������õ������ǿ���������α���յ��α������ԭ����
	   ������α꣬����ֱ����ӡ�

	���⣬������������յ��α�����Ч�α꣬���յ��α���Ҫ�滻��ԭ������յ��αꡣ
*/
const yuki_region* yuki_region_manager::allocFromSubRegion(yuki_file_string* buffer, 
	const yuki_region* originalRegion, const yuki_cursor& startPos, const yuki_cursor& endPos, 
	int indent, yuki_region_type type /*= Yuki_linedRegion*/)
{
	if (originalRegion->getRegionType() != Yuki_linedRegion || type == Yuki_linedRegion)
		type = Yuki_blockRegion;

	yuki_region* region = m_allocator->allocRegion(type, startPos, endPos, indent);

	const yuki_cursor& originalStartCursor = originalRegion->begin();
	buffer->addCursorOffsetToCursor(region->startCursor, originalStartCursor);
	buffer->addCursorOffsetToCursor(region->endCursor, originalStartCursor);
	if (!region->endCursor.isValid())
		region->endCursor = originalRegion->end();
	region->indent += originalRegion->getIndent();

	return region;
}
