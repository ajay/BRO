#include <iostream>
#include <stack>
#include <queue>
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
		void createPreviousBrownMaze();
		void DFS();

	protected:
		int size;
		cell maze[SIZE*SIZE];
};