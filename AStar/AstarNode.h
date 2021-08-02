#pragma once

#include "Node.h"
#include <vector>

using namespace std;

class AstarNode: public Node
{
private:
	int g; // distance from start
	int h; // distance between this node to the target node

	int calculateH(int* targetIndex, int* startIndex) const
	{
		return abs(targetIndex[0] - startIndex[0]) + abs(targetIndex[1] - startIndex[1]);
	}

public:
	AstarNode(int r, int c, AstarNode* pr, int* targetIndex, int* startIndex);
	AstarNode(int r, int c, AstarNode* pr, int g, int* targetIndex, int* startIndex);

	int getG() const { return this->g; }
	int getH() const { return this->h; }

	bool operator>(const AstarNode& other) const { return (this->g + this->h) > (other.g + other.h); }
};

