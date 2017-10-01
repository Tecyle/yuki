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
	__inline virtual yuki_structure_type getNodeType() const { return m_nodeType; }
	__inline virtual const wchar_t* getNodeName() const { return m_nodeName; }

	__inline virtual yuki_node* getParent() { return m_parent; }
	__inline virtual int getChildrenCount() { return m_children.size(); }
	__inline virtual yuki_node* getChild(int index);
	__inline virtual const vector<yuki_node*>& getChildren() const { return m_children; }

	__inline virtual const yuki_region* getRegion() const { return m_region; }

	__inline virtual void appendChild(yuki_node* child);

protected:
	const wchar_t* m_nodeName;				///< 节点的名称，用来标识节点
	yuki_structure_type m_nodeType;			///< 节点类型

	yuki_node* m_parent;					///< 节点的父节点，顶层节点此值为 nullptr
	vector<yuki_node*> m_children;			///< 节点的子节点列表

	yuki_region* m_region;					///< 该节点所包括的区域
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