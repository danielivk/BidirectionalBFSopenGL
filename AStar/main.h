#pragma once

#include "glut.h"
#include "AstarNode.h"
#include "CompareAstarNodes.h"

#include <time.h>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

void setupMaze();
void init();
void drawMaze();
void buildPath(Node* pCurrent);
void BFSIteration();
void display();
void idle();
void menu(int choice);


