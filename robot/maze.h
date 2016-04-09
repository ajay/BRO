#include <iostream>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <time.h>

#include "cell.h"

#define ULTRA_FRONT 0
#define ULTRA_LEFT  2
#define ULTRA_RIGHT  3

using namespace std;

#define SIZE 6
#define threshold 5

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

		stack <cell> neighbors;
		stack <cell> used;


		int getX(int last);
		int getY(int last);
		int _i(int x, int y);

	protected:
		int size;
		cell maze[SIZE*SIZE];
};