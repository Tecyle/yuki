#pragma once

class YukiLinedRegion : public YukiRegion, virtual public YukiDynamicClass
{
public:
	YukiLinedRegion(const YukiLinedRegion* region);
	YukiLinedRegion(yuki_cursor beginCursor, yuki_cursor endCursor, int indentLevel = -1);
};

class YukiBlockRegion : public YukiRegion, virtual public YukiDynamicClass
{
public:
	YukiBlockRegion(const YukiBlockRegion* region);
};