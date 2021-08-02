#pragma once

class Node
{
protected:
	int row, col;
	Node* parent;

public:
	/* Types:
		SPACE = 0;
		WALL = 1;
		START = 2;
		TARGET = 3;
		BLACK_SPACE = 4; // Visited and already checked points
		GRAY = 5; // Casing (shape scope) = The unvisited current test points
		PATH = 6; // Shortest path from start node to target node
	*/
	enum Type { WALL, SPACE, START, PATH, GRAY, BLACK_SPACE, TARGET };

	Node(int r, int c, Node* pr);

	int getRow() const			{ return this->row; }
	int getCol() const			{ return this->col; }
	Node* getParent() const		{ return this->parent; }

	void setRow(int row);
	void setCol(int col);
	void setParent(Node* p);

	friend class main;
};

