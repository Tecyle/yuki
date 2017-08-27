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

class YukiStruct
{
public:
	YukiStruct(YukiFileReader* fileReader, const YukiBlockRegion* region, YukiStruct* parent);
	~YukiStruct();

public:
	// ���ӹ�ϵ
	virtual __inline YukiStruct* getParent();
	virtual bool appendChild(YukiStruct* child);
	virtual __inline YukiStruct* getChild(int index);
	virtual __inline int getChildrenCount();

public:
	// ��Ա��ϵ
	virtual __inline const wchar_t* getName() const;
	virtual __inline yuki_struct_type getType() const;
	virtual __inline YukiNode* getNode();
	virtual __inline YukiMatcher* getMatcher();
	virtual __inline const YukiRegion* getRegion();

public:
	// ����
	virtual bool parse() = 0;

protected:
	const wchar_t* m_name;
	yuki_struct_type m_type;
	const YukiRegion* m_region;
	YukiFileReader* m_fileReader;
	YukiStruct* m_parent;
	vector<YukiStruct*> m_children;
};

