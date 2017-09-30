#pragma once

/*
	yuki 节点：
	语法解析的结果会生成一个节点树，代表了整个文档的逻辑结构。
*/

class yuki_node
{
protected:
	yuki_node();
	virtual ~yuki_node();

public:
	virtual yuki_structure_type getNodeType() const { return m_nodeType; }
	virtual const wchar_t* getNodeName() const { return m_nodeName; }

	virtual yuki_node* getParent() { return m_parent; }
	virtual int getChildrenCount() { return m_children.size(); }
	virtual yuki_node* getChild(int index);
	virtual const vector<yuki_node*>& getChildren() const { return m_children; }

	virtual const yuki_region* getRegion() const { return m_region; }

	virtual __inline void appendChild(yuki_node* child);

protected:
	const wchar_t* m_nodeName;
	yuki_structure_type m_nodeType;

	yuki_node* m_parent;
	vector<yuki_node*> m_children;

	yuki_region* m_region;
};

class YukiWalkerAction
{
public:
	// 在访问 currentNode 的子节点之前调用
	virtual void beforeVisit(yuki_node* root, yuki_node* currentNode) = 0;
	// 在访问 currentNode 的子节点之后调用
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