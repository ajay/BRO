#include <iostream>
#include <stack>
#include <stdlib.h>
#include <time.h>

#include "cell.h"

using namespace std;

class Maze
{
	public:
		Maze(int);
		~Maze();
		void printMaze();
		void createMaze();
		void DFS();

	protected:
		int size;
		Cell* blocks;
};