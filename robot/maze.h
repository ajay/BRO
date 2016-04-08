#include <iostream>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <time.h>

#include "cell.h"

using namespace std;

#define SIZE 6

class Maze
{
	public:
		Maze();
		~Maze();
		void printMaze();
		void createMaze();
		void createMiniMaze();
		void createPreviousBrownMaze();
		void DFS();

		std::vector<string> path_instructions;
		std::vector<int> path;

		int getX(int last);
		int getY(int last);
		int _i(int x, int y);

	protected:
		int size;
		cell maze[SIZE*SIZE];
};