#include "TObjectNode.h"

TObjectNode::TObjectNode(UINT l)
{
	level = l;
}

TObjectNode::~TObjectNode()
{
}

UINT TObjectNode::GetLevel()
{
	return level;
}

void TObjectNode::SetParent(TrecPointerSoft<TObjectNode> p)
{
	parentNode = p;
}
