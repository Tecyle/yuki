#pragma once

/*
	������Ŀ����н����������� Hyperlink Target �ṹ
	�����������£�

	1. �������пո񣬲��Ჹ�κοո�
	2. ���Կ�Խ����
	3. ת��հ׻ᵱ�ɿո�
	4. ���һ���ַ�����Ƿ�ת���»��ߣ��͵����Ǳ�������
	5. ���������Ϊ�գ������ʧ��
*/
class YukiHyperlinkTargetNode;

class YukiLinkUri : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;

protected:
	virtual bool match() override { return false; }
	void parseAliasTarget(YukiHyperlinkTargetNode* node);
	void parseUrlTarget(YukiHyperlinkTargetNode* node);
};