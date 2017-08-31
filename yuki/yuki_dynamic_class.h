#pragma once

// TODO 这里有一个遗留问题，释放内存的时候不会去调用析构函数
class YukiDynamicClass
{
public:
	void* operator new (size_t size)
	{
		if (size == 0)   // 处理 0 byte 申请
			size = 1;
		
		void* pMem = YukiMemoPools::getInstance()->mallocInPools(size);
		if (pMem)
			return pMem;
			
		// 分配失败  
		// 获取当前的new_handler  
		new_handler globalHandler = set_new_handler(0);
		set_new_handler(globalHandler);
		
		if (globalHandler) 
			(*globalHandler)();	
		else 
			throw std::bad_alloc();
	}

	void operator delete(void*)
	{
		// 内存由 YukiMemoPools 统一释放，所以在这里什么都不做
		return;
	}
};