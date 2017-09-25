#pragma once

/*
	使用该类的时候需要注意，不会去调用对象的析构函数。
*/
template<class T, typename... ConstructorParams>
class yuki_object_allocator
{
public:
	yuki_object_allocator();
	~yuki_object_allocator();

public:
	__inline T* allocObject(ConstructorParams... params);

	__inline T* cloneObject(const T& obj);

	__inline void freeObject(T* obj);

protected:
	void extendPoolIfNeeded();
	__inline T* getValidObject();

protected:
	typedef vector<T*> yuki_object_pool;
	yuki_object_pool m_objectPool;
	yuki_object_pool m_releasedObejctPool;
	int m_usedPosition;
	static const int m_poolSize = 1024;
};
