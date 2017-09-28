#pragma once
#include "yuki_file_loader.h"

/*
	����һ�� yuki �ĵ��Ľ����ṹ����Ҫ�����������ݣ�


*/

enum yuki_struct_type
{
	Yuki_listType,			 ///< �б�����
	Yuki_blockType,			 ///< ������
	Yuki_imageType,			 ///< ͼ������
	Yuki_tableType,			 ///< �������
	Yuki_formulaType,		 ///< ��ʽ����
	Yuki_inlineMarkType,	 ///< �����������
	Yuki_controlType		 ///< ��ʽ��������
};

class YukiStruct : virtual public YukiDynamicClass
{
public:
	YukiStruct(YukiGlobal* globalData);
	~YukiStruct();

public:
	// ����
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
