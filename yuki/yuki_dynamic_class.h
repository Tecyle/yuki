#pragma once

// TODO ������һ���������⣬�ͷ��ڴ��ʱ�򲻻�ȥ������������
class YukiDynamicClass
{
public:
	void* operator new (size_t size)
	{
		if (size == 0)   // ���� 0 byte ����
			size = 1;
		
		void* pMem = YukiMemoPools::getInstance()->mallocInPools(size);
		if (pMem)
			return pMem;
			
		// ����ʧ��  
		// ��ȡ��ǰ��new_handler  
		new_handler globalHandler = set_new_handler(0);
		set_new_handler(globalHandler);
		
		if (globalHandler) 
			(*globalHandler)();	
		else 
			throw std::bad_alloc();
	}

	void operator delete(void*)
	{
		// �ڴ��� YukiMemoPools ͳһ�ͷţ�����������ʲô������
		return;
	}
};