#pragma once
#include "yuki_file_loader.h"

/*
	定义一个 yuki 文档的解析结构，需要定义以下内容：


*/

enum yuki_struct_type
{
	Yuki_listType,			 ///< 列表类型
	Yuki_blockType,			 ///< 块类型
	Yuki_imageType,			 ///< 图像类型
	Yuki_tableType,			 ///< 表格类型
	Yuki_formulaType,		 ///< 公式类型
	Yuki_inlineMarkType,	 ///< 内联标记类型
	Yuki_controlType		 ///< 格式控制类型
};

class YukiStruct
{
public:
	YukiStruct(YukiFileReader* fileReader, const YukiBlockRegion* region, YukiStruct* parent);
	~YukiStruct();

public:
	// 父子关系
	virtual __inline YukiStruct* getParent();
	virtual bool appendChild(YukiStruct* child);
	virtual __inline YukiStruct* getChild(int index);
	virtual __inline int getChildrenCount();

public:
	// 成员关系
	virtual __inline const wchar_t* getName() const;
	virtual __inline yuki_struct_type getType() const;
	virtual __inline YukiNode* getNode();
	virtual __inline YukiMatcher* getMatcher();
	virtual __inline const YukiRegion* getRegion();

public:
	// 解析
	virtual bool parse() = 0;

protected:
	const wchar_t* m_name;
	yuki_struct_type m_type;
	const YukiRegion* m_region;
	YukiFileReader* m_fileReader;
	YukiStruct* m_parent;
	vector<YukiStruct*> m_children;
};

