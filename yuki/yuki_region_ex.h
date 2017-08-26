#pragma once

class YukiLinedRegion : public YukiRegion
{
public:
	YukiLinedRegion(const YukiLinedRegion* region);
};

class YukiBlockRegion : public YukiRegion
{
public:
	YukiBlockRegion(const YukiBlockRegion* region);
};