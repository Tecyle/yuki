#pragma once

/*
	��������
*/
enum yuki_region_type
{
	yuki_linedRegion,		///< ����������
	yuki_blockRegion		///< ��״����
};

/*
	�ṹ����
*/
enum yuki_structure_type
{
	yuki_listStructure,			///< �б�����
	yuki_blockStructure,		///< ������
	yuki_imageStructure,		///< ͼ������
	yuki_tableStructure,		///< �������
	yuki_formulaStructure,		///< ��ʽ����
	yuki_inlineMarkStructure,	///< �����������
	yuki_controlStructure		///< ��ʽ��������
};
