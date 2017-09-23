#pragma once

class YukiLinedRegion : public yuki_region, virtual public YukiDynamicClass
{
public:
	YukiLinedRegion(const YukiLinedRegion* region);
	YukiLinedRegion(yuki_cursor beginCursor, yuki_cursor endCursor, int indentLevel = -1);
};

class YukiBlockRegion : public yuki_region, virtual public YukiDynamicClass
{
public:
	YukiBlockRegion(const YukiBlockRegion* region);
};