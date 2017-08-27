#pragma once

class YukiFileReader;

class YukiMatcher
{
public:
	/*
		匹配函数，需要满足：

		* 匹配成功之后，要移动光标到匹配块的末尾
		* 匹配得到的区间要在 region 参数中返回
		* 匹配成功返回 true，匹配失败返回 false，不需要手动还原光标
	*/
	virtual bool match(YukiFileReader* reader, YukiRegion* region) = 0;
};

class YukiMatcherManager
{

};