#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_node.h"

void YukiNodeWalker::walk(yuki_node* root)
{
	m_rootNode = root;

	visit(root);
}

void YukiNodeWalker::visit(yuki_node* root)
{
	YukiWalkerAction* action = m_actionCollection->getAction(root->getNodeName());
	action->beforeVisit(m_rootNode, root);
	for (auto node : root->getChildren())
	{
		visit(node);
	}
	action->afterVisit(m_rootNode, root);
}

void yuki_node::appendChild(yuki_node* child)
{
	assert(child->m_parent == nullptr);
	child->m_parent = this;
	m_children.push_back(child);
}
