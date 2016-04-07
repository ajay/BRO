#include <iostream>
#include <stack>
#include <stdlib.h>
#include <time.h>

#include "cell.h"

using namespace std;

#define SIZE 16

class Maze
{
	public:
		Maze();
		~Maze();
		void printMaze();
		void createMaze();
		void DFS();
		cell maze[SIZE][SIZE];
};