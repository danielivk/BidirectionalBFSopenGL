#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <cstdlib>


#include "GLUT.h"
#include <math.h>
#include <time.h>
#include "Node.h"
#include <vector>
#include "graph.h"
#include <queue>
#include "CompareNodes.h"
using namespace std;

/*___________________________________Daniel Ivkovich: 316421262_________________________________*/


/*___________________________________________Constants___________________________________________*/		
const int W = 600; // window width
const int H = 600; // window height
const int MSZ = 80;
const int NUM_NODES = 60;
const int NUM_EDGES = 200;

bool run_b_bfs = false, reset=false, sleep=false;


/*___________________________________Variables for Algorithm_________________________________*/

Node* maze[MSZ][MSZ];
Graph graph;
vector <Node*> gray;
vector <Node*> gray_from_source;
vector <Node*> gray_from_target;
priority_queue <Node*, vector<Node*>, CompareNodes> pq;
int trg_count = 0;
int src_count = 0;



/*___________________________________General Functions Signatures_________________________________*/

int GetTargetLocation();
void CreateGraph();
void displayGraph();
void resetGraph();
void RestorePathInGraph(Node* first, int searchVal);
void Menu(int choice);
void idle();
void init();



/*___________________________________Main_________________________________*/

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(600, 200);
	glutCreateWindow("BFS Bidirectional Simulation");

	glutIdleFunc(idle); // idle: when nothing happens

	// menu
	glutCreateMenu(Menu);
	glutAddMenuEntry("BFS Bidirectional", 1);
	glutAddMenuEntry("Reset", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();
	glutMainLoop();
}

void init()
{
	int i, j;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
			maze[i][j] = new Node();

	srand(time(0)); // pseudo randomizer
	glClearColor(0, 0, 0, 0);
	CreateGraph();
	glutDisplayFunc(displayGraph); // refresh function
	glOrtho(-1, 1, -1, 1, -1, 1);
}
void resetGraph()
{
	src_count = 0;
	trg_count = 0;
	reset = false;

	while (!pq.empty())
		pq.pop();
	while (!gray.empty())
		gray.pop_back();
	while (!gray_from_source.empty())
		gray_from_source.pop_back();
	while (!gray_from_target.empty())
		gray_from_target.pop_back();
	graph.resetGraph();
	init();
}



double Distance(int index1, int index2)
{
	double x1, x2, y1, y2;

	x1 = graph.GetNode(index1)->GetColumn();
	y1 = graph.GetNode(index1)->GetLine();
	x2 = graph.GetNode(index2)->GetColumn();
	y2 = graph.GetNode(index2)->GetLine();

	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void CreateGraph()
{
	int i, j, k, m=0;
	Node* pn;
	Edge* pe1, *pe2;
	double tmp;

	// add random Nodes
	for (k = 0; k < NUM_NODES; k++)
	{
		pn = new Node();
		pn->SetLine((MSZ / 10) + rand() % (MSZ - MSZ/5));
		pn->SetColumn((MSZ / 10) + rand() % (MSZ - MSZ / 5));
		pn->SetValue(NEUTRAL);

		graph.AddNode(pn);

	}
	// add random Edges
	for (k = 0; k < NUM_EDGES; k++)
	{
		do {
			i = rand() % NUM_NODES;
			j = rand() % NUM_NODES;
			tmp = Distance(i, j);

		} while (tmp > 15);
		
		// add edges to graph
		pe1 = new Edge(i, j, tmp);
		pe2 = new Edge(j, i, tmp);

		graph.AddEdge(pe1);
		graph.AddEdge(pe2);
		// add edges to outgoing of nodes
		graph.GetNode(i)->AddEdge2Outgoing(pe1);
		graph.GetNode(j)->AddEdge2Outgoing(pe2);
	}

	graph.GetNode(0)->SetValue(SRC);
	
	graph.GetNode(GetTargetLocation())->SetValue(TARGET);
	// add start node to priority queue
	pq.push(graph.GetNode(0));
	gray_from_source.push_back(graph.GetNode(0));
	gray_from_target.push_back(graph.GetNode(graph.GetNodes().size() - 1));
	graph.GetNode(0)->SetDisToTarget(Distance(GetTargetLocation(),0));
}

int GetTargetLocation()
{
	return graph.GetNodes().size() - 1;
}

void DrawNode(int i, int j)
{
	double sz, x, y, radius;

	sz = 2.0 / MSZ;
	radius = sz;
	x = j * sz - 1;
	y = i * sz - 1;
	double twicePi = 2.0 * 3.142;
	glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
	glVertex2f(x + sz/2, y + sz/2); // center of circle
	int index, numberOfPoints = 50;
	for (index = 0; index <= numberOfPoints; index++) {
		glVertex2f((x + (radius * cos(index * twicePi / numberOfPoints))), (y + (radius * sin(index * twicePi / numberOfPoints))));
	}
	glEnd();

}

void DrawEdge(Edge* pe)
{
	double x1, y1, x2, y2, sz;

	sz = 2.0 / MSZ;

	x1 = graph.GetNode(pe->GetFirst())->GetColumn() * sz - 1 + sz / 2;
	y1 = graph.GetNode(pe->GetFirst())->GetLine()* sz - 1 + sz / 2;
	x2 = graph.GetNode(pe->GetSecond())->GetColumn()* sz - 1 + sz / 2;
	y2 = graph.GetNode(pe->GetSecond())->GetLine()* sz - 1 + sz / 2;

	glColor3d(1, 1, 1); // white

	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();


}
void reDrawEdge(Edge* pe)
{
	double x1, y1, x2, y2, sz;

	sz = 2.0 / MSZ;

	x1 = graph.GetNode(pe->GetFirst())->GetColumn() * sz - 1 + sz / 2;
	y1 = graph.GetNode(pe->GetFirst())->GetLine() * sz - 1 + sz / 2;
	x2 = graph.GetNode(pe->GetSecond())->GetColumn() * sz - 1 + sz / 2;
	y2 = graph.GetNode(pe->GetSecond())->GetLine() * sz - 1 + sz / 2;

	glColor3d(0, 1, 0); // green

	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();


}



void DrawGraph()
{
	int i;
	Node* node;
	// draw nodes
	for (i = 0; i < NUM_NODES; i++)
	{
		node = graph.GetNode(i);
		for (auto it : graph.GetNode(i)->GetOutgoing())
			DrawEdge(it);
		switch (node->GetValue())
		{
		case NEUTRAL:
			glColor3d(1, 1, 1); // white
			break;
		case SRC:
			glColor3d(0, 0, 1); // light blue
			break;
		case TARGET:
			for each (Edge * e in node->GetOutgoing())
			{
				int val = graph.GetNode(e->GetSecond())->GetValue();
				bool b = val == PATH;
				if (b)
				{
					reDrawEdge(e);
					break;
				}
			}
			glColor3d(1, 0, 0); // red
			break;
		case VISITED_SRC:
			glColor3d(0.5, 0, 0.5);
			break;
		case VISITED_TGT:
			glColor3d(0.5, 0.5, 0);
			break;
		case GRAY_SRC:
			glColor3d(0, 0, 0.5); // dark blue
			break;
		case GRAY_TRG:
			glColor3d(0.5, 0, 0); // dark red

			break;
		case PATH:
			for each(Edge* e in node->GetOutgoing())
			{
				int val1 = graph.GetNode(e->GetFirst())->GetValue();
				int val2 = graph.GetNode(e->GetSecond())->GetValue();
				bool b1 = val1 == PATH || val1 == SRC || val1 == TARGET;
				bool b2 = val2 == PATH || val2 == SRC || val2 == TARGET;

				if (b1 && b2)
				{
					sleep = true;
					reDrawEdge(e);
				}
			}
			glColor3d(0, 1, 0); // green
			break;
		}//switch
		DrawNode(graph.GetNode(i)->GetLine(), graph.GetNode(i)->GetColumn());
		
	}

	
}

void RestorePath(Node* first)
{
	Node* current = new Node(first);

	while (current->GetValue() != SRC)
	{
		maze[current->GetLine()][current->GetColumn()]->SetValue(PATH);
		current = current->GetParent();
	}
}

void RestorePathInGraph(Node* first)
{
	Node* current = first;
	while (current->GetValue() != SRC)
	{
		// paint it to PATH color
		if (current->GetValue() != TARGET)
		{
			current->SetValue(PATH);
		}
		current = current->GetParent();

	}

}

void RestorePathInGraph(Node* first,int searchVal)
{
	Node* current = first;
	while (current->GetValue() != searchVal)
	{
		// paint it to PATH color
		current->SetValue(PATH);
		current = current->GetParent();

	}

}


void BfsBidirectionalIteration()
{
	
	

	if (gray_from_source.size() == src_count || gray_from_target.size() == trg_count) // there is no way to target
	{
		run_b_bfs = false;
		cout << "no path found"<< endl;
	}
	else
	{
		Node* currentFromSource;
		Node* tmp;
		currentFromSource = gray_from_source[src_count];
		src_count++;

		if(currentFromSource->GetValue()!=SRC)
			currentFromSource->SetValue(VISITED_SRC);

		for each (Edge* edge in currentFromSource->GetOutgoing())
		{
			tmp = graph.GetNode(edge->GetSecond());
			if (tmp->GetValue() == TARGET)
			{
				run_b_bfs = false;
				return;
			}
			else if (tmp->GetValue() == GRAY_TRG)
			{
				run_b_bfs = false;
				RestorePathInGraph(tmp, TARGET);
				RestorePathInGraph(currentFromSource, SRC);
				return;
			}
			else if (tmp->GetValue() == NEUTRAL)
			{
				tmp->SetParent(currentFromSource);
				tmp->SetValue(GRAY_SRC);
				gray_from_source.push_back(tmp);
			}
		}
	

		Node* currentFromTarget;
		currentFromTarget = gray_from_target[trg_count];
		trg_count++;

		if (currentFromTarget->GetValue() != TARGET)
			currentFromTarget->SetValue(VISITED_TGT);

		for each (Edge* edge in currentFromTarget->GetOutgoing())
		{
			tmp = graph.GetNode(edge->GetSecond());
			if (tmp->GetValue() == SRC)
			{
				run_b_bfs = false;
				return;
			}
			else if (tmp->GetValue() == GRAY_SRC)
			{
				run_b_bfs = false;
				RestorePathInGraph(tmp, SRC);
				RestorePathInGraph(currentFromTarget, TARGET);
				return;
			}
			else if (tmp->GetValue() == NEUTRAL)
			{
				tmp->SetParent(currentFromTarget);
				tmp->SetValue(GRAY_TRG);
				gray_from_target.push_back(tmp);
			}
		}
	}



}







void displayGraph()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawGraph();
	if (sleep)
	{
		Sleep(100);
		sleep = false;
	}

	glutSwapBuffers();// show what was drawn in "frame buffer"
}



void idle()
{	
	if (run_b_bfs)
	{
		BfsBidirectionalIteration();
		Sleep(250);
	}
	if (reset)
		resetGraph();
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	switch (choice) {
	case 1:
		run_b_bfs = true;
		break;	
	case 2:
		reset = true;
		break;
	}
}


