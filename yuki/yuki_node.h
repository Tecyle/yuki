#pragma once

/*
	yuki �ڵ㣺
	�﷨�����Ľ��������һ���ڵ����������������ĵ����߼��ṹ��
*/

class YukiNode
{
protected:
	YukiNode(YukiNode* parent, const YukiRegion* region);

public:
	virtual yuki_struct_type getNodeType() const { return m_nodeType; }
	virtual const wchar_t* getNodeName() const { return m_nodeName; }

	virtual YukiNode* getParent() { return m_parent; }
	virtual int getChildrenCount() { return m_children.size(); }
	virtual YukiNode* getChild(int index);
	virtual const vector<YukiNode*>& getChildren() const { return m_children; }

	virtual const YukiRegion* getRegion() const { return m_region; }

protected:
	const wchar_t* m_nodeName;
	yuki_struct_type m_nodeType;

	YukiNode* m_parent;
	vector<YukiNode*> m_children;

	YukiRegion* m_region;
};

class YukiWalkerAction
{
public:
	// �ڷ��� currentNode ���ӽڵ�֮ǰ����
	virtual void beforeVisit(YukiNode* root, YukiNode* currentNode) = 0;
	// �ڷ��� currentNode ���ӽڵ�֮�����
	virtual void afterVisit(YukiNode* root, YukiNode* currentNode) = 0;
};

class YukiWalkerActionCollection
{
public:
	virtual YukiWalkerAction* getAction(const wchar_t* nodeName);
};

class YukiNodeWalker
{
public:
	void walk(YukiNode* root);

protected:
	void visit(YukiNode* root);

private:
	YukiWalkerActionCollection* m_actionCollection;
	YukiNode* m_rootNode;
};