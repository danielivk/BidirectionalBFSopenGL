#pragma once

#include "AstarNode.h"

class CompareAstarNodes
{
public:
	bool operator()(AstarNode* pn1, AstarNode* pn2) const { return pn1 > pn2; }
};

