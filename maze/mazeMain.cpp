#include "maze.h"

using namespace std;

int main()
{
	Maze* bruhhhh = new Maze();
	// bruhhhh->createMaze();
	bruhhhh->createPreviousBrownMaze();
	// cout<<"Maze"<<endl;
	// bruhhhh->printMaze();
	// cout<<"DFS"<<endl;
	bruhhhh->DFS();
}