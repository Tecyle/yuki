#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_region_ex.h"

YukiLinedRegion::YukiLinedRegion(const YukiLinedRegion* region)
	: yuki_region(region)
{
}

YukiBlockRegion::YukiBlockRegion(const YukiBlockRegion* region)
	: yuki_region(region)
{
}
