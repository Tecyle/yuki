#pragma once

class YukiMemoPools
{
public:
	static YukiMemoPools* getInstance();

	void* mallocInPools(size_t size);
	void freeInPools(void* p);
};