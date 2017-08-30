#pragma once

/*
	yuki 节点：
	语法解析的结果会生成一个节点树，代表了整个文档的逻辑结构。
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
	// 在访问 currentNode 的子节点之前调用
	virtual void beforeVisit(YukiNode* root, YukiNode* currentNode) = 0;
	// 在访问 currentNode 的子节点之后调用
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