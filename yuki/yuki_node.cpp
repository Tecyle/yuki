#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_node.h"

void YukiNodeWalker::walk(YukiNode* root)
{
	m_rootNode = root;

	visit(root);
}

void YukiNodeWalker::visit(YukiNode* root)
{
	YukiWalkerAction* action = m_actionCollection->getAction(root->getNodeName());
	action->beforeVisit(m_rootNode, root);
	for (auto node : root->getChildren())
	{
		visit(node);
	}
	action->afterVisit(m_rootNode, root);
}
