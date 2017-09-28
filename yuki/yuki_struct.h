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

class YukiStruct : virtual public YukiDynamicClass
{
public:
	YukiStruct(YukiGlobal* globalData);
	~YukiStruct();

public:
	// 解析
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) = 0;
	virtual bool match() = 0;

	__inline const wchar_t* getName() const { return m_name; }

protected:
	__inline YukiStruct* getParser(const wchar_t* name);
	__inline yuki_file_reader* getFileReader();
	__inline YukiMatcherCollection* getMatcherCollection();
	__inline YukiMatcher* getMatcher(const wchar_t* name);

protected:
	const wchar_t* m_name;
	yuki_struct_type m_type;
	YukiGlobal* m_globalData;
};

class YukiStructCollection
{
public:
	YukiStructCollection(YukiGlobal* globalData);

public:
	__inline YukiString* getStruct(const wchar_t* name);

private:
	StringHashmap<YukiStruct*> m_structMap;
};
