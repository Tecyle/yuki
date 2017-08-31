#pragma once

class YukiLinedRegion : public YukiRegion, virtual public YukiDynamicClass
{
public:
	YukiLinedRegion(const YukiLinedRegion* region);
};

class YukiBlockRegion : public YukiRegion, virtual public YukiDynamicClass
{
public:
	YukiBlockRegion(const YukiBlockRegion* region);
};