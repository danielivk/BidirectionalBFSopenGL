#include "AstarNode.h"

AstarNode::AstarNode(int r, int c, AstarNode* pr, int* targetIndex, int* startIndex) : Node(r, c, pr)
{
	this->g = 0;
	this->h = calculateH(targetIndex, startIndex);
}

AstarNode::AstarNode(int r, int c, AstarNode* pr, int g, int* targetIndex, int* startIndex) : Node(r, c, pr)
{
	this->g = g;
	this->h = calculateH(targetIndex, startIndex);
}
