#include "Node.h"
#include <stdio.h>



Node::Node()
{
	value = NEUTRAL;
	parent = nullptr;
	g = 0;
	disToTarget = -1;
}


Node::Node(Node *other)
{
	value = other->GetValue();
	line = other->GetLine();
	column = other->GetColumn();
	g = 0;
	if (other->GetParent() == nullptr)
		parent = nullptr;
	else
		parent = new Node(other->GetParent());
	disToTarget = -1;
}

Node::Node(Node *other, double g)
{
	this->g = g;
	value = other->GetValue();
	line = other->GetLine();
	column = other->GetColumn();
	g = 0;
	if (other->GetParent() == nullptr)
		parent = nullptr;
	else
		parent = new Node(other->GetParent());
	disToTarget = -1;

}
Node::~Node()
{
}


void Node::SetParent(Node* p)
{
	parent = p;
}


void Node::SetValue( int value)
{
	this->value = value;
}

void Node::SetLine(int line)
{
	this->line = line;
}


void Node::SetColumn(int col)
{
	this->column = col;
}

int Node::GetValue()
{
	return value;
}


int Node::GetLine()
{
	return line;
}


int Node::GetColumn()
{
	return column;
}

Node* Node::GetParent()
{
	return parent;
}

void Node::AddEdge2Outgoing(Edge* pe) {
	outgoing.push_back(pe);
}

vector <Edge*> Node::GetOutgoing()
{
	return outgoing;
}

void Node::SetDisToTarget(double distance)
{
	this->disToTarget = distance;
}

double Node::GetDisToTarget()
{
	return this->disToTarget;
}

double Node::GetG() { // distance grom start
	return g; 
}

void Node::SetG(double g) {
	this->g = g;
}

