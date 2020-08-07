#pragma once
#include <vector>
#include "Edge.h"


using namespace std;
const int NEUTRAL = 0;
const int SRC = 1;
const int TARGET = 2;
const int PATH = 3; // belongs to the path to target
const int GRAY = 4;
const int VISITED_SRC = 5; 
const int VISITED_TGT = 6;

const int GRAY_SRC = 7; 
const int GRAY_TRG = 8; 


class Node
{
public:
	Node();
	Node( Node *other);
	Node(Node * other, double g);
	~Node();

private:
	int value;
	Node* parent;
	int line, column;
	double g; // distance from start
	vector <Edge*> outgoing;
	double disToTarget; //distance of current to target

public:
	void SetParent(Node * p);
	void SetValue(int value);
	void SetLine(int line);
	void SetColumn(int col);
	int GetValue();
	int GetLine();
	int GetColumn();
	Node * GetParent();
	void AddEdge2Outgoing(Edge * pe);
	vector<Edge*> GetOutgoing();
	void SetDisToTarget(double distance);
	double GetDisToTarget();
	double GetG();

	void SetG(double g);

};

