#include "maze.h"

using namespace std;

int main()
{
	Maze* maze = new Maze();
	maze->createMaze();
	cout<<"Maze"<<endl;
	maze->printMaze();
	cout<<"DFS"<<endl;
	maze->DFS();
}