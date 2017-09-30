#pragma once

/*
	区域类型
*/
enum yuki_region_type
{
	yuki_linedRegion,		///< 行连续区域
	yuki_blockRegion		///< 块状区域
};

/*
	结构类型
*/
enum yuki_structure_type
{
	yuki_listStructure,			///< 列表类型
	yuki_blockStructure,		///< 块类型
	yuki_imageStructure,		///< 图像类型
	yuki_tableStructure,		///< 表格类型
	yuki_formulaStructure,		///< 公式类型
	yuki_inlineMarkStructure,	///< 内联标记类型
	yuki_controlStructure		///< 格式控制类型
};
