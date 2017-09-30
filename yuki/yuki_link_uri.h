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

class YukiLinkUri : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;

protected:
	virtual bool match() override { return false; }
	void parseAliasTarget(YukiHyperlinkTargetNode* node);
	void parseUrlTarget(YukiHyperlinkTargetNode* node);
};