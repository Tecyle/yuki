#pragma once

/*
	yuki �ڵ㣺
	�﷨�����Ľ��������һ���ڵ����������������ĵ����߼��ṹ��
*/

class yuki_node
{
protected:
	yuki_node();
	virtual ~yuki_node();

public:
	__inline virtual yuki_structure_type getNodeType() const { return m_nodeType; }
	__inline virtual const wchar_t* getNodeName() const { return m_nodeName; }

	__inline virtual yuki_node* getParent() { return m_parent; }
	__inline virtual int getChildrenCount() { return m_children.size(); }
	__inline virtual yuki_node* getChild(int index);
	__inline virtual const vector<yuki_node*>& getChildren() const { return m_children; }

	__inline virtual const yuki_region* getRegion() const { return m_region; }

	__inline virtual void appendChild(yuki_node* child);

protected:
	const wchar_t* m_nodeName;				///< �ڵ�����ƣ�������ʶ�ڵ�
	yuki_structure_type m_nodeType;			///< �ڵ�����

	yuki_node* m_parent;					///< �ڵ�ĸ��ڵ㣬����ڵ��ֵΪ nullptr
	vector<yuki_node*> m_children;			///< �ڵ���ӽڵ��б�

	yuki_region* m_region;					///< �ýڵ�������������
};

class YukiWalkerAction
{
public:
	// �ڷ��� currentNode ���ӽڵ�֮ǰ����
	virtual void beforeVisit(yuki_node* root, yuki_node* currentNode) = 0;
	// �ڷ��� currentNode ���ӽڵ�֮�����
	virtual void afterVisit(yuki_node* root, yuki_node* currentNode) = 0;
};

class YukiWalkerActionCollection
{
public:
	virtual YukiWalkerAction* getAction(const wchar_t* nodeName);
};

class YukiNodeWalker
{
public:
	void walk(yuki_node* root);

protected:
	void visit(yuki_node* root);

private:
	YukiWalkerActionCollection* m_actionCollection;
	yuki_node* m_rootNode;
};