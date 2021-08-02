#include "Node.h"

Node::Node(int r, int c, Node* pr)
{
    this->row = r;
    this->col = c;
    this->parent = pr;
}

void Node::setRow(int row)
{
    this->row = row;
}

void Node::setCol(int col)
{
    this->col = col;
}

void Node::setParent(Node* parent)
{
    this->parent = parent;
}