#include "main.h"

const int W = 600; // window width
const int H = 600; // window height
const int MSZ = 100;

int maze[MSZ][MSZ];

bool start_BFS = false;
bool start_astar = false;

// Visited --> BFS nodes
vector <Node*> gray;

// We need a priority queue to store gray nodes (by distance from start)
// A* nodes
priority_queue <AstarNode*, vector<AstarNode*>, CompareAstarNodes> pq_gray;
int startIndex[2] = { MSZ / 2, MSZ / 2 };
int targetIndex[2];

void init()
{
	// RED, GREEN, BLUEs
	glClearColor((GLclampf) 0.8, 0, 0, 0); // color of window background

	srand((unsigned int) time(0));

	// define main axes
	glOrtho(-1, 1, -1, 1, -1, 1); 

	targetIndex[0] = rand() % MSZ;
	targetIndex[1] = rand() % MSZ;
	setupMaze();
}

void setupMaze()
{
	int i, j;

	// set borders
	for (i = 0; i < MSZ; i++)
	{
		maze[i][0] = Node::Type::WALL;
		maze[i][MSZ - 1] = Node::Type::WALL;
		maze[0][i] = Node::Type::WALL;
		maze[MSZ - 1][i] = Node::Type::WALL;
	}

	for (i = 1; i < MSZ - 1; i++)
	{
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1) // mostly Node::Type::SPACEs (90%)
			{
				if (rand() % 10 > 0) maze[i][j] = Node::Type::SPACE;
				else maze[i][j] = Node::Type::WALL;
			}
			else // mostly Node::Type::WALLs (70%)
			{
				if (rand() % 10 > 2)maze[i][j] = Node::Type::WALL;
				else maze[i][j] = Node::Type::SPACE;
			}
		}
	}

	// set start point
	maze[startIndex[0]][startIndex[1]] = Node::Type::START;

	// set target point
	maze[targetIndex[0]][targetIndex[1]] = Node::Type::TARGET;
	
	// insert to gray the Node
	AstarNode* pn = new AstarNode(startIndex[0], startIndex[1], nullptr, targetIndex, startIndex);
	gray.push_back(pn);
	pq_gray.push(pn);
}

void drawMaze() 
{
	int i, j;
	double dx, dy, x, y;

	dx = dy = 2.0 / MSZ; // 2.0 is the logical width and height of the window

	y = -1;
	for (i = 0; i < MSZ; i++, y+=dy)
	{
		x = -1;
		for (j = 0; j < MSZ; j++, x+=dx)
		{
			switch (maze[i][j]) 
			{
			case Node::Type::WALL:
				glColor3d(0.2, 0.2, 0.2);
				break;
			case Node::Type::SPACE:
				glColor3d(1, 1, 1);
				break;
			case Node::Type::START:
				glColor3d(0.2, 0.4, 1);
				break;
			case Node::Type::TARGET:
				glColor3d(1, 0, 0);
				break;
			case Node::Type::BLACK_SPACE:
				glColor3d(1, 1, 0);
				break;
			case Node::Type::GRAY:
				glColor3d(0, 0.8, 0);
				break;
			case Node::Type::PATH:
				glColor3d(1, 0.5, 1);
				break;
			}

			// Draw one square in the maze
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y+dy);
			glVertex2d(x+dx, y+dy);
			glVertex2d(x+dx, y);
			glEnd();
		}
	}
}

void buildPath(Node* pCurrent)
{
	while (maze[pCurrent->getRow()][pCurrent->getCol()] != Node::Type::START)
	{
		maze[pCurrent->getRow()][pCurrent->getCol()] = Node::Type::PATH;
		pCurrent = pCurrent->getParent();
	}
}

void BFSIteration()
{
	Node* pCurrent;
	Node* next;

	if (gray.empty()) // there is no solution
	{
		cout << "there is no solution" << endl;
		start_BFS = false;
	}
	else
	{
		// 1. extract first element from gray
		pCurrent = gray[0];
		gray.erase(gray.begin());

		// 2. paint pCurrent black
		if(maze[pCurrent->getRow()][pCurrent->getCol()] != Node::Type::START)
			maze[pCurrent->getRow()][pCurrent->getCol()] = Node::Type::BLACK_SPACE;

		// 3. add white neighbours of pCurrent to gray
		// check up
		if (maze[pCurrent->getRow() + 1][pCurrent->getCol()] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow() + 1][pCurrent->getCol()] = Node::Type::GRAY;
			next = new Node(pCurrent->getRow() + 1, pCurrent->getCol(), pCurrent);
			gray.push_back(next);
		}
		else if (maze[pCurrent->getRow() + 1][pCurrent->getCol()] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_BFS = false;
			buildPath(pCurrent);
		}

		// check right
		if (maze[pCurrent->getRow() ][pCurrent->getCol()+ 1] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow() ][pCurrent->getCol()+ 1] = Node::Type::GRAY;
			next = new Node(pCurrent->getRow(), pCurrent->getCol() + 1, pCurrent);
			gray.push_back(next);
		}
		else if (maze[pCurrent->getRow()][pCurrent->getCol() + 1] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_BFS = false;
			buildPath(pCurrent);
		}

		// check down
		if (maze[pCurrent->getRow() - 1][pCurrent->getCol()] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow() - 1][pCurrent->getCol()] = Node::Type::GRAY;
			next = new Node(pCurrent->getRow() - 1, pCurrent->getCol(), pCurrent);
			gray.push_back(next);
		}
		else if (maze[pCurrent->getRow() - 1][pCurrent->getCol()] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_BFS = false;
			buildPath(pCurrent);
		}

		// check up
		if (maze[pCurrent->getRow() + 1][pCurrent->getCol()] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow() + 1][pCurrent->getCol()] = Node::Type::GRAY;
			next = new Node(pCurrent->getRow() + 1, pCurrent->getCol(), pCurrent);
			gray.push_back(next);
		}
		else if (maze[pCurrent->getRow() + 1][pCurrent->getCol()] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_BFS = false;
			buildPath(pCurrent);
		}

		// check left
		if (maze[pCurrent->getRow()][pCurrent->getCol() - 1] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow()][pCurrent->getCol() - 1] = Node::Type::GRAY;
			next = new Node(pCurrent->getRow(), pCurrent->getCol() - 1, pCurrent);
			gray.push_back(next);
		}
		else if (maze[pCurrent->getRow()][pCurrent->getCol() - 1] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_BFS = false;
			buildPath(pCurrent);
		}

		// check right
		if (maze[pCurrent->getRow()][pCurrent->getCol() + 1] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow()][pCurrent->getCol() + 1] = Node::Type::GRAY;
			next = new Node(pCurrent->getRow(), pCurrent->getCol() + 1, pCurrent);
			gray.push_back(next);
		}
		else if (maze[pCurrent->getRow()][pCurrent->getCol() + 1] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_BFS = false;
			buildPath(pCurrent);
		}
	}
}

void AStarIteration()
{
	AstarNode* pCurrent;
	AstarNode* next;

	if (pq_gray.empty()) // there is no solution
	{
		cout << "there is no solution" << endl;
		start_astar = false;
	}
	else
	{
		// 1. extract first element from gray
		pCurrent = pq_gray.top();
		pq_gray.pop();

		// 2. paint pCurrent black
		if (maze[pCurrent->getRow()][pCurrent->getCol()] != Node::Type::START)
			maze[pCurrent->getRow()][pCurrent->getCol()] = Node::Type::BLACK_SPACE;

		// 3. add white neighbours of pCurrent to gray
		// check up
		if (maze[pCurrent->getRow() + 1][pCurrent->getCol()] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			int b[2] = { MSZ / 2 , MSZ / 2 };
			maze[pCurrent->getRow() + 1][pCurrent->getCol()] = Node::Type::GRAY;
			next = new AstarNode(pCurrent->getRow() + 1, pCurrent->getCol(), pCurrent, pCurrent->getG() + 1, targetIndex, startIndex);
			pq_gray.push(next);
		}
		else if (maze[pCurrent->getRow() + 1][pCurrent->getCol()] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_astar = false;
			buildPath(pCurrent);
		}

		// check right
		if (maze[pCurrent->getRow()][pCurrent->getCol() + 1] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow()][pCurrent->getCol() + 1] = Node::Type::GRAY;
			next = new AstarNode(pCurrent->getRow(), pCurrent->getCol() + 1, pCurrent, pCurrent->getG() + 1, targetIndex, startIndex); // create it
			pq_gray.push(next);
		}
		else if (maze[pCurrent->getRow()][pCurrent->getCol() + 1] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_astar = false;
			buildPath(pCurrent);
		}

		// check down
		if (maze[pCurrent->getRow() - 1][pCurrent->getCol()] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow() - 1][pCurrent->getCol()] = Node::Type::GRAY;
			next = new AstarNode(pCurrent->getRow() - 1, pCurrent->getCol(), pCurrent, pCurrent->getG() + 1, targetIndex, startIndex); // create it
			pq_gray.push(next);
		}
		else if (maze[pCurrent->getRow() - 1][pCurrent->getCol()] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_astar = false;
			buildPath(pCurrent);
		}

		// check left
		if (maze[pCurrent->getRow()][pCurrent->getCol() - 1] == Node::Type::SPACE) // Node::Type::SPACE means that it is WHITE
		{
			maze[pCurrent->getRow()][pCurrent->getCol() - 1] = Node::Type::GRAY;
			next = new AstarNode(pCurrent->getRow(), pCurrent->getCol() - 1, pCurrent, pCurrent->getG() + 1, targetIndex, startIndex); // create it
			pq_gray.push(next);
		}
		else if (maze[pCurrent->getRow()][pCurrent->getCol() - 1] == Node::Type::TARGET)
		{
			cout << "the solution has been found" << endl;
			start_astar = false;
			buildPath(pCurrent);
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	drawMaze();
	
	glutSwapBuffers(); // show all
}

void idle()
{
	if (start_BFS)
		BFSIteration();
	else if (start_astar)
		AStarIteration();

	glutPostRedisplay(); // call to display again
}

void menu(int choice)
{
	if (choice == 1) // BFS
	{
		start_BFS = true;
	}
	else if (choice == 2) // A*
	{
		start_astar = true;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Search the red dot in a maze");

	glutDisplayFunc(display); // Refresh window function
	glutIdleFunc(idle); // Kind of timer function

	glutCreateMenu(menu);
	glutAddMenuEntry("BFS", 1);
	glutAddMenuEntry("A*", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();

	exit(0);
}