#include "stdafx.h"
#include "yuki_object_allocator.h"

template<class T, typename... ConstructorParams>
yuki_object_allocator::yuki_object_allocator()
{
	m_usedPosition = 0;
}

template<class T, typename... ConstructorParams>
yuki_object_allocator::~yuki_object_allocator()
{
	for (auto obj : m_objectPool)
	{
		free(obj);
	}
}

template<class T, typename... ConstructorParams>
T* yuki_object_allocator::allocObject(ConstructorParams... params)
{
	return new(getValidObject()) T(std::forward<ConstructorParams>(params)...);
}

template<class T, typename... ConstructorParams>
T* yuki_object_allocator::cloneObject(const T& obj)
{
	return new(getValidObject()) T(obj);
}

template<class T, typename... ConstructorParams>
void yuki_object_allocator::freeObject(T* obj)
{
	obj->~T();
	m_releasedObejctPool.push_back(obj);
}

template<class T, typename... ConstructorParams>
void yuki_object_allocator::extendPoolIfNeeded()
{
	if (!m_releasedObejctPool.empty())
		return;

	if (m_usedPosition < m_poolSize)
		return;

	T* obj = (T*)malloc(sizeof(T) * m_poolSize);
	m_objectPool.push_back(obj);
	m_usedPosition = 0;
}

template<class T, typename... ConstructorParams>
T* yuki_object_allocator::getValidObject()
{
	extendPoolIfNeeded();

	T* obj = nullptr;
	// 先查找释放池里面是否有可用的，没有可用的就扩展一个
	if (m_releasedObejctPool.empty())
	{
		obj = m_objectPool[m_usedPosition++];
	}
	else
	{
		obj = m_releasedObejctPool.back();
		m_releasedObejctPool.pop_back();
	}

	assert(obj);
	return obj;
}
