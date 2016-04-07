#include "maze.h"
#include <vector>
#include <algorithm>

using namespace std;

// Create a maze of given size (sizeXsize)
Maze::Maze()
{
	size = SIZE;
}

// Destructor for the maze
Maze::~Maze()
{
}

int getX(int last)
{
	while (last >= SIZE)
	{
		last -= SIZE;
	}
	return last;
}

int getY(int last)
{
	int i = 0;
	while (last >= SIZE)
	{
		i++;
		last -= SIZE;
	}
	return i;
}

int _i(int x, int y)
{
	return 16*y + x;
}

// Print the maze once constructed
void Maze::printMaze()
{
	// Print the top border
	cout << "+  ";
	for (int r = 1; r < size; r++)
	{
		cout <<"+--";
	}
	cout <<"+"<<endl;

	int index = 0;
	while(index<size)
	{
		// print the edges and spaces
		for(int r = 0; r < size; r++)
		{
			if(maze[(size*index)+r].getWest() == NULL || r == 0)
				cout<<"|  ";
			else
				cout<<"   ";
		}

		cout<<"|"<<endl;

		// print the bottom border of a row
		if(index == size-1)
		{
			for(int r = 0; r < size; r++)
			{
				if(r == size - 1)
					cout<<"+  ";
				else
					cout<<"+--";
			}
			cout<<"+"<<endl;
		}
		else // last row
		{
			for(int r = 0; r < size; r++)
			{
				if(maze[(size*index)+r].getSouth()!=NULL)
				{
					cout<<"+  ";
				}
				else
				{
					cout<<"+--";
				}
			}
			cout <<"+"<<endl;
		}
		index++;
	}
	cout <<endl;
}


void Maze::createPreviousBrownMaze()
{
	std::vector<int> walls[256];

	// North, South, West, East
	// 1 means wall, 0 means no wall
	// Row 0
	walls[_i( 0,  0)] = {1, 1, 1, 0};
	walls[_i( 1,  0)] = {1, 1, 0, 0};
	walls[_i( 2,  0)] = {1, 0, 0, 1};
	walls[_i( 3,  0)] = {1, 0, 1, 0};
	walls[_i( 4,  0)] = {1, 1, 0, 0};
	walls[_i( 5,  0)] = {1, 1, 0, 0};
	walls[_i( 6,  0)] = {1, 0, 0, 0};
	walls[_i( 7,  0)] = {1, 1, 0, 0};
	walls[_i( 8,  0)] = {1, 1, 0, 0};
	walls[_i( 9,  0)] = {1, 0, 0, 0};
	walls[_i(10,  0)] = {1, 1, 0, 0};
	walls[_i(11,  0)] = {1, 0, 0, 0};
	walls[_i(12,  0)] = {1, 1, 0, 0};
	walls[_i(13,  0)] = {1, 1, 0, 0};
	walls[_i(14,  0)] = {1, 1, 0, 0};
	walls[_i(15,  0)] = {1, 0, 0, 1};

	// Row 1
	walls[_i( 0,  1)] = {1, 0, 1, 0};
	walls[_i( 1,  1)] = {1, 0, 0, 0};
	walls[_i( 2,  1)] = {0, 1, 0, 0};
	walls[_i( 3,  1)] = {0, 1, 0, 0};
	walls[_i( 4,  1)] = {1, 1, 0, 0};
	walls[_i( 5,  1)] = {1, 0, 0, 0};
	walls[_i( 6,  1)] = {0, 1, 0, 0};
	walls[_i( 7,  1)] = {1, 0, 0, 1};
	walls[_i( 8,  1)] = {1, 0, 1, 0};
	walls[_i( 9,  1)] = {0, 1, 0, 0};
	walls[_i(10,  1)] = {1, 0, 0, 0};
	walls[_i(11,  1)] = {0, 1, 0, 1};
	walls[_i(12,  1)] = {1, 0, 1, 1};
	walls[_i(13,  1)] = {1, 0, 1, 0};
	walls[_i(14,  1)] = {1, 1, 0, 0};
	walls[_i(15,  1)] = {0, 0, 0, 1};

	// Row 2
	walls[_i( 0,  2)] = {0, 0, 1, 1};
	walls[_i( 1,  2)] = {0, 0, 1, 1};
	walls[_i( 2,  2)] = {1, 0, 1, 1};
	walls[_i( 3,  2)] = {1, 0, 1, 0};
	walls[_i( 4,  2)] = {1, 0, 0, 0};
	walls[_i( 5,  2)] = {0, 1, 0, 0};
	walls[_i( 6,  2)] = {1, 1, 0, 0};
	walls[_i( 7,  2)] = {0, 1, 0, 0};
	walls[_i( 8,  2)] = {0, 1, 0, 0};
	walls[_i( 9,  2)] = {1, 1, 0, 0};
	walls[_i(10,  2)] = {0, 1, 0, 1};
	walls[_i(11,  2)] = {1, 0, 1, 0};
	walls[_i(12,  2)] = {0, 1, 0, 0};
	walls[_i(13,  2)] = {0, 0, 0, 1};
	walls[_i(14,  2)] = {1, 0, 1, 0};
	walls[_i(15,  2)] = {0, 0, 0, 1};

	// Row 3
	walls[_i( 0,  3)] = {0, 0, 1, 1};
	walls[_i( 1,  3)] = {0, 0, 1, 1};
	walls[_i( 2,  3)] = {0, 0, 1, 1};
	walls[_i( 3,  3)] = {0, 1, 1, 1};
	walls[_i( 4,  3)] = {0, 0, 1, 1};
	walls[_i( 5,  3)] = {1, 0, 1, 0};
	walls[_i( 6,  3)] = {1, 1, 0, 0};
	walls[_i( 7,  3)] = {1, 0, 0, 0};
	walls[_i( 8,  3)] = {1, 1, 0, 0};
	walls[_i( 9,  3)] = {1, 0, 0, 0};
	walls[_i(10,  3)] = {1, 0, 0, 1};
	walls[_i(11,  3)] = {0, 1, 1, 0};
	walls[_i(12,  3)] = {1, 0, 0, 1};
	walls[_i(13,  3)] = {0, 1, 1, 0};
	walls[_i(14,  3)] = {0, 1, 0, 1};
	walls[_i(15,  3)] = {0, 0, 1, 1};

	// Row 4
	walls[_i( 0,  4)] = {0, 0, 1, 1};
	walls[_i( 1,  4)] = {0, 1, 1, 0};
	walls[_i( 2,  4)] = {0, 1, 0, 0};
	walls[_i( 3,  4)] = {1, 1, 0, 0};
	walls[_i( 4,  4)] = {0, 1, 0, 1};
	walls[_i( 5,  4)] = {0, 0, 1, 0};
	walls[_i( 6,  4)] = {1, 1, 0, 0};
	walls[_i( 7,  4)] = {0, 0, 0, 0};
	walls[_i( 8,  4)] = {1, 1, 0, 0};
	walls[_i( 9,  4)] = {0, 0, 0, 1};
	walls[_i(10,  4)] = {0, 0, 1, 0};
	walls[_i(11,  4)] = {1, 1, 0, 0};
	walls[_i(12,  4)] = {0, 0, 0, 1};
	walls[_i(13,  4)] = {1, 0, 1, 0};
	walls[_i(14,  4)] = {1, 0, 0, 1};
	walls[_i(15,  4)] = {0, 0, 1, 0};

	// Row 5
	walls[_i( 0,  5)] = {0, 0, 1, 0};
	walls[_i( 1,  5)] = {1, 1, 0, 1};
	walls[_i( 2,  5)] = {1, 0, 1, 0};
	walls[_i( 3,  5)] = {1, 1, 0, 0};
	walls[_i( 4,  5)] = {1, 0, 0, 1};
	walls[_i( 5,  5)] = {0, 0, 1, 0};
	walls[_i( 6,  5)] = {1, 1, 0, 0};
	walls[_i( 7,  5)] = {0, 1, 0, 0};
	walls[_i( 8,  5)] = {1, 1, 0, 0};
	walls[_i( 9,  5)] = {0, 1, 0, 0};
	walls[_i(10,  5)] = {0, 0, 0, 0};
	walls[_i(11,  5)] = {1, 0, 0, 0};
	walls[_i(12,  5)] = {0, 0, 0, 1};
	walls[_i(13,  5)] = {0, 0, 1, 1};
	walls[_i(14,  5)] = {0, 1, 1, 1};
	walls[_i(15,  5)] = {0, 0, 1, 1};

	// Row 6
	walls[_i( 0,  6)] = {0, 0, 1, 1};
	walls[_i( 1,  6)] = {1, 0, 1, 0};
	walls[_i( 2,  6)] = {0, 1, 0, 1};
	walls[_i( 3,  6)] = {1, 0, 1, 0};
	walls[_i( 4,  6)] = {0, 1, 0, 1};
	walls[_i( 5,  6)] = {0, 0, 1, 1};
	walls[_i( 6,  6)] = {1, 0, 1, 0};
	walls[_i( 7,  6)] = {1, 1, 0, 0};
	walls[_i( 8,  6)] = {1, 1, 0, 0};
	walls[_i( 9,  6)] = {1, 0, 0, 1};
	walls[_i(10,  6)] = {0, 0, 1, 1};
	walls[_i(11,  6)] = {0, 0, 1, 1};
	walls[_i(12,  6)] = {0, 0, 1, 1};
	walls[_i(13,  6)] = {0, 1, 1, 0};
	walls[_i(14,  6)] = {1, 0, 0, 1};
	walls[_i(15,  6)] = {0, 0, 1, 1};

	// Row 7
	walls[_i( 0,  7)] = {0, 0, 1, 0};
	walls[_i( 1,  7)] = {0, 1, 0, 1};
	walls[_i( 2,  7)] = {1, 0, 1, 0};
	walls[_i( 3,  7)] = {0, 1, 0, 1};
	walls[_i( 4,  7)] = {1, 0, 1, 0};
	walls[_i( 5,  7)] = {0, 1, 0, 1};
	walls[_i( 6,  7)] = {0, 0, 1, 1};
	walls[_i( 7,  7)] = {1, 0, 1, 0};
	walls[_i( 8,  7)] = {1, 0, 0, 1};
	walls[_i( 9,  7)] = {0, 1, 1, 0};
	walls[_i(10,  7)] = {0, 0, 0, 1};
	walls[_i(11,  7)] = {0, 0, 1, 0};
	walls[_i(12,  7)] = {0, 0, 0, 1};
	walls[_i(13,  7)] = {1, 0, 1, 0};
	walls[_i(14,  7)] = {0, 1, 0, 0};
	walls[_i(15,  7)] = {0, 0, 0, 1};

	// Row 8
	walls[_i( 0,  8)] = {0, 0, 1, 1};
	walls[_i( 1,  8)] = {1, 0, 1, 0};
	walls[_i( 2,  8)] = {0, 1, 0, 1};
	walls[_i( 3,  8)] = {1, 0, 1, 0};
	walls[_i( 4,  8)] = {0, 1, 0, 1};
	walls[_i( 5,  8)] = {1, 0, 1, 0};
	walls[_i( 6,  8)] = {0, 0, 0, 1};
	walls[_i( 7,  8)] = {0, 1, 1, 0};
	walls[_i( 8,  8)] = {0, 1, 0, 0};
	walls[_i( 9,  8)] = {1, 0, 0, 1};
	walls[_i(10,  8)] = {0, 0, 1, 1};
	walls[_i(11,  8)] = {0, 0, 1, 1};
	walls[_i(12,  8)] = {0, 1, 1, 0};
	walls[_i(13,  8)] = {0, 0, 0, 0};
	walls[_i(14,  8)] = {1, 1, 0, 0};
	walls[_i(15,  8)] = {0, 0, 0, 1};

	// Row 9
	walls[_i( 0,  9)] = {0, 0, 1, 1};
	walls[_i( 1,  9)] = {0, 0, 1, 1};
	walls[_i( 2,  9)] = {1, 0, 1, 0};
	walls[_i( 3,  9)] = {0, 0, 0, 1};
	walls[_i( 4,  9)] = {1, 0, 1, 0};
	walls[_i( 5,  9)] = {0, 1, 0, 1};
	walls[_i( 6,  9)] = {0, 0, 1, 0};
	walls[_i( 7,  9)] = {1, 0, 0, 1};
	walls[_i( 8,  9)] = {1, 0, 1, 0};
	walls[_i( 9,  9)] = {0, 1, 0, 1};
	walls[_i(10,  9)] = {0, 0, 1, 1};
	walls[_i(11,  9)] = {0, 0, 1, 1};
	walls[_i(12,  9)] = {1, 0, 1, 0};
	walls[_i(13,  9)] = {0, 1, 0, 0};
	walls[_i(14,  9)] = {1, 0, 0, 0};
	walls[_i(15,  9)] = {0, 0, 0, 1};

	// Row 10
	walls[_i( 0, 10)] = {0, 0, 1, 1};
	walls[_i( 1, 10)] = {0, 0, 1, 1};
	walls[_i( 2, 10)] = {0, 0, 1, 1};
	walls[_i( 3, 10)] = {0, 0, 1, 0};
	walls[_i( 4, 10)] = {0, 1, 0, 0};
	walls[_i( 5, 10)] = {1, 0, 0, 0};
	walls[_i( 6, 10)] = {0, 1, 0, 1};
	walls[_i( 7, 10)] = {0, 1, 1, 0};
	walls[_i( 8, 10)] = {0, 1, 0, 0};
	walls[_i( 9, 10)] = {1, 0, 0, 0};
	walls[_i(10, 10)] = {0, 0, 0, 1};
	walls[_i(11, 10)] = {0, 0, 1, 1};
	walls[_i(12, 10)] = {0, 0, 1, 0};
	walls[_i(13, 10)] = {1, 1, 0, 0};
	walls[_i(14, 10)] = {0, 0, 0, 1};
	walls[_i(15, 10)] = {0, 0, 1, 1};

	// Row 11
	walls[_i( 0, 11)] = {0, 0, 1, 1};
	walls[_i( 1, 11)] = {0, 0, 1, 1};
	walls[_i( 2, 11)] = {0, 0, 1, 1};
	walls[_i( 3, 11)] = {0, 0, 1, 0};
	walls[_i( 4, 11)] = {1, 0, 0, 1};
	walls[_i( 5, 11)] = {0, 1, 1, 0};
	walls[_i( 6, 11)] = {1, 0, 0, 1};
	walls[_i( 7, 11)] = {1, 0, 1, 0};
	walls[_i( 8, 11)] = {1, 1, 0, 0};
	walls[_i( 9, 11)] = {0, 1, 0, 1};
	walls[_i(10, 11)] = {0, 1, 1, 0};
	walls[_i(11, 11)] = {0, 0, 0, 1};
	walls[_i(12, 11)] = {0, 0, 1, 0};
	walls[_i(13, 11)] = {1, 0, 0, 1};
	walls[_i(14, 11)] = {0, 0, 1, 1};
	walls[_i(15, 11)] = {0, 0, 1, 1};

	// Row 12
	walls[_i( 0, 12)] = {0, 0, 1, 1};
	walls[_i( 1, 12)] = {0, 0, 1, 1};
	walls[_i( 2, 12)] = {0, 0, 1, 1};
	walls[_i( 3, 12)] = {0, 0, 1, 1};
	walls[_i( 4, 12)] = {0, 1, 1, 0};
	walls[_i( 5, 12)] = {1, 0, 0, 1};
	walls[_i( 6, 12)] = {0, 1, 1, 0};
	walls[_i( 7, 12)] = {0, 1, 0, 1};
	walls[_i( 8, 12)] = {1, 0, 1, 0};
	walls[_i( 9, 12)] = {1, 0, 0, 0};
	walls[_i(10, 12)] = {1, 0, 0, 0};
	walls[_i(11, 12)] = {0, 1, 0, 1};
	walls[_i(12, 12)] = {0, 0, 1, 1};
	walls[_i(13, 12)] = {0, 0, 1, 0};
	walls[_i(14, 12)] = {0, 1, 0, 1};
	walls[_i(15, 12)] = {0, 0, 1, 1};

	// Row 13
	walls[_i( 0, 13)] = {0, 0, 1, 1};
	walls[_i( 1, 13)] = {0, 0, 1, 1};
	walls[_i( 2, 13)] = {0, 0, 1, 1};
	walls[_i( 3, 13)] = {0, 1, 1, 0};
	walls[_i( 4, 13)] = {1, 1, 0, 0};
	walls[_i( 5, 13)] = {0, 1, 0, 0};
	walls[_i( 6, 13)] = {1, 1, 0, 0};
	walls[_i( 7, 13)] = {1, 1, 0, 0};
	walls[_i( 8, 13)] = {0, 1, 0, 1};
	walls[_i( 9, 13)] = {0, 0, 1, 1};
	walls[_i(10, 13)] = {0, 1, 1, 0};
	walls[_i(11, 13)] = {1, 0, 0, 0};
	walls[_i(12, 13)] = {0, 1, 0, 0};
	walls[_i(13, 13)] = {0, 0, 0, 0};
	walls[_i(14, 13)] = {1, 1, 0, 0};
	walls[_i(15, 13)] = {0, 0, 0, 1};

	// Row 14
	walls[_i( 0, 14)] = {0, 0, 1, 1};
	walls[_i( 1, 14)] = {0, 1, 1, 0};
	walls[_i( 2, 14)] = {0, 1, 0, 0};
	walls[_i( 3, 14)] = {1, 1, 0, 0};
	walls[_i( 4, 14)] = {1, 0, 0, 0};
	walls[_i( 5, 14)] = {1, 1, 0, 0};
	walls[_i( 6, 14)] = {1, 0, 0, 0};
	walls[_i( 7, 14)] = {1, 1, 0, 0};
	walls[_i( 8, 14)] = {1, 0, 0, 1};
	walls[_i( 9, 14)] = {0, 1, 1, 0};
	walls[_i(10, 14)] = {1, 1, 0, 1};
	walls[_i(11, 14)] = {0, 0, 1, 1};
	walls[_i(12, 14)] = {1, 0, 1, 1};
	walls[_i(13, 14)] = {0, 0, 1, 0};
	walls[_i(14, 14)] = {1, 1, 0, 0};
	walls[_i(15, 14)] = {0, 0, 0, 1};

	// Row 15
	walls[_i( 0, 15)] = {0, 1, 1, 0};
	walls[_i( 1, 15)] = {1, 1, 0, 0};
	walls[_i( 2, 15)] = {1, 1, 0, 0};
	walls[_i( 3, 15)] = {1, 1, 0, 0};
	walls[_i( 4, 15)] = {0, 1, 0, 0};
	walls[_i( 5, 15)] = {1, 1, 0, 0};
	walls[_i( 6, 15)] = {0, 1, 0, 0};
	walls[_i( 7, 15)] = {1, 1, 0, 0};
	walls[_i( 8, 15)] = {0, 1, 0, 0};
	walls[_i( 9, 15)] = {1, 1, 0, 0};
	walls[_i(10, 15)] = {1, 1, 0, 0};
	walls[_i(11, 15)] = {0, 1, 0, 0};
	walls[_i(12, 15)] = {0, 1, 0, 0};
	walls[_i(13, 15)] = {0, 1, 0, 0};
	walls[_i(14, 15)] = {1, 1, 0, 0};
	walls[_i(15, 15)] = {0, 1, 0, 1};

	// // Row #
	// walls[_i( 0, #)] = {0, 0, 0, 0};
	// walls[_i( 1, #)] = {0, 0, 0, 0};
	// walls[_i( 2, #)] = {0, 0, 0, 0};
	// walls[_i( 3, #)] = {0, 0, 0, 0};
	// walls[_i( 4, #)] = {0, 0, 0, 0};
	// walls[_i( 5, #)] = {0, 0, 0, 0};
	// walls[_i( 6, #)] = {0, 0, 0, 0};
	// walls[_i( 7, #)] = {0, 0, 0, 0};
	// walls[_i( 8, #)] = {0, 0, 0, 0};
	// walls[_i( 9, #)] = {0, 0, 0, 0};
	// walls[_i(10, #)] = {0, 0, 0, 0};
	// walls[_i(11, #)] = {0, 0, 0, 0};
	// walls[_i(12, #)] = {0, 0, 0, 0};
	// walls[_i(13, #)] = {0, 0, 0, 0};
	// walls[_i(14, #)] = {0, 0, 0, 0};
	// walls[_i(15, #)] = {0, 0, 0, 0};

	for (int c = 0; c < 256; c++)
	{
		cell *north = (walls[c][0] == 1) ? NULL : &maze[_i(getX(c), getY(c)-1)];
		cell *south = (walls[c][1] == 1) ? NULL : &maze[_i(getX(c), getY(c)+1)];
		cell *west  = (walls[c][2] == 1) ? NULL : &maze[_i(getX(c)-1, getY(c))];
		cell *east  = (walls[c][3] == 1) ? NULL : &maze[_i(getX(c)+1, getY(c))];
		maze[c].setWalls(north, south, west, east);
	}
}



// Create a random maze
void Maze::createMaze()
{
	stack <int> cellStack;
	int Totalcells = size * size;

	int i;
	int Currentcell = 0;
	int Visitedcells = 1;
	srand(time(NULL));

	while(Visitedcells < Totalcells)
	{
		int nb[4] = {0, 0, 0, 0}; // make an array to store which neighbors a cell has
		int num = 0;

		// Check if cell has neighbors
		// Check North
		if (Currentcell-size >= 0 && maze[Currentcell-size].getVisited() == 0)
		{
			nb[num] = 1;
			num++;
		}

		// Check East
		if ((Currentcell % size < size -1) && maze[Currentcell+1].getVisited() == 0)
		{
			nb[num] = 2;
			num++;
		}

		// Check South
		if (Currentcell < Totalcells - size && maze[Currentcell+size].getVisited() == 0)
		{
			nb[num] = 3;
			num++;
		}

		// Check West
		if (Currentcell % size != 0 && maze[Currentcell-1].getVisited() == 0)
		{
			nb[num] = 4;
			num++;
		}

		// if cell has neighbors, pick one and "knock down the wall between them"
		if(num > 0)
		{
			int choice = nb[rand()%(num)];
			switch(choice)
			{
				case 1:
					i = Currentcell-size;
					maze[Currentcell].setNorth(&maze[i]);
					maze[i].setSouth(&maze[Currentcell]);
					maze[Currentcell].setVisited(1);
					maze[i].setVisited(1);
					cellStack.push(Currentcell);
					Currentcell = i;
					Visitedcells++;
					break;

				case 2:
					i = Currentcell + 1;
					maze[Currentcell].setEast(&maze[i]);
					maze[i].setWest(&maze[Currentcell]);
					maze[Currentcell].setVisited(1);
					maze[i].setVisited(1);
					cellStack.push(Currentcell);
					Currentcell =i;
					Visitedcells++;
					break;

				case 3:
					i = Currentcell + size;
					maze[Currentcell].setSouth(&maze[i]);
					maze[i].setNorth(&maze[Currentcell]);
					maze[Currentcell].setVisited(1);
					maze[i].setVisited(1);
					cellStack.push(Currentcell);
					Currentcell =i;
					Visitedcells++;
					break;

				case 4:
					i = Currentcell - 1;
					maze[Currentcell].setWest(&maze[i]);
					maze[i].setEast(&maze[Currentcell]);
					maze[Currentcell].setVisited(1);
					maze[i].setVisited(1);
					cellStack.push(Currentcell);
					Currentcell =i;
					Visitedcells++;
					break;

				default:
					break;
			}
		}
		else // cell has no neighbors, pop off the stack
		{
			maze[Currentcell].setVisited(2);
			Currentcell = cellStack.top();
			cellStack.pop();
		}
	}
	// reset all marks to 0
	for (int i = 0; i < Totalcells; i++)
	{
		maze[i].setVisited(0);
	}
 }

// CHANGED TO BFS FROM DFS
// DFS function to traverse the maze and find the shortest path
void Maze::DFS()
{
	queue<int> s;
	int i = 0;
	int j;
	s.push(i);
	int num = 0;
	int reset = 0;

	int goal = _i(7, 7);

	while(s.empty()==false)
	{
		i = s.front();
		s.pop();
		if(num == 0 && reset == 0)
			maze[i].setParent(NULL);

		maze[i].setNum(num);

		// if the current cell is the exit cell, break out of the loop
		if(maze[i].getVisited() == 0)
		{
			if(i == goal)
			{
				// break;
			}
		}

		// do DFS and set parent
		if(maze[i].getEast() != NULL && maze[i].getEast()->getVisited()==0)
		{
			s.push(i+1);
			maze[i+1].setParent(&maze[i]);
		}
		if(maze[i].getSouth() != NULL && maze[i].getSouth()->getVisited()==0)
		{
			s.push(i+size);
			maze[i+size].setParent(&maze[i]);
		}
		if(maze[i].getWest() != NULL && maze[i].getWest()->getVisited()==0)
		{
			s.push(i-1);
			maze[i-1].setParent(&maze[i]);
		}
		if(maze[i].getNorth() != NULL && maze[i].getNorth()->getVisited()==0)
		{
			s.push(i-size);
			maze[i-size].setParent(&maze[i]);
		}
		maze[i].setVisited(1);
		num++;
		if(num ==10){
			num = 0;
			reset++;
		}
	}

	// print out maze with DFS order of cells visited

	/*
	cout << "+  ";
	for (int r = 1; r < size; r++)
	{
		cout <<"+--";
	}
	cout <<"+"<<endl;

	int index = 0;
	while(index<size)
	{
		for(int r = 0; r < size; r++)
		{
			if(maze[(size*index)+r].getWest() == NULL || r == 0)
			{
				if(maze[(size*index)+r].getNum() >= 0)
					cout<<"| "<<maze[(size*index)+r].getNum();
				else
					cout<<"|  ";
			}
			else
			{
				if(maze[(size*index)+r].getNum() >= 0)
				{
					cout<<"  "<<maze[(size*index)+r].getNum();
				}
				else
				{
					cout<<"   ";
				}
			}
		}
		cout<<"|"<<endl;

		if(index == size-1)
		{
			for(int r = 0; r < size; r++)
			{
				if(r == size - 1)
				{
					cout<<"+  ";
				}
				else
				{
					cout<<"+--";
				}
			}
			cout<<"+"<<endl;
		}
		else
		{
			for(int r = 0; r < size; r++)
			{
				if(maze[(size*index)+r].getSouth()!=NULL)
				{
					cout<<"+  ";
				}
				else
				{
					cout<<"+--";
				}
			}
			cout <<"+"<<endl;
		}
		index++;
	}

	cout <<endl;

	*/

	//traverse starting from the end cell to find the shortest path
	// cout <<"Shortest Path"<<endl;
	int last = goal;

	std::vector<int> path;
	path.push_back(goal);

	while(last != 0)
	{
		maze[last].setSym('#');
		if(&maze[last-size] == maze[last].getParent())
		{
			maze[last-size].setSouthSym('V');
			last = last-size;
		}
		else if(&maze[last+1] == maze[last].getParent())
		{
			maze[last+1].setWestSym('<');
			last = last+1;
		}
		else if(&maze[last+size] == maze[last].getParent())
		{
			maze[last].setSouthSym('^');
			last = last+size;
		}
		else
		{
			maze[last].setWestSym('>');
			last = last-1;
		}

		path.push_back(last);
	}
	maze[0].setSym('#');

	std::reverse(path.begin(), path.end());

	// path.push_back(goal);

	std::vector<int> path_dir;

	for (unsigned int i = 1; i < path.size(); i++)
	{
		if (getX(path[i]) - getX(path[i-1]) == 1)
		{
			path_dir.push_back(1);
		}
		else if (getX(path[i]) - getX(path[i-1]) == -1)
		{
			path_dir.push_back(3);
		}
		else if (getY(path[i]) - getY(path[i-1]) == 1)
		{
			path_dir.push_back(2);
		}
		else if (getY(path[i]) - getY(path[i-1]) == -1)
		{
			path_dir.push_back(0);
		}
	}

	path_dir.push_back(5);

	std::vector<string> path_instructions;

	path_instructions.push_back("STRAIGHT");

	for (unsigned int i = 1; i < path_dir.size(); i++)
	{
		if (path_dir[i] - path_dir[i-1] == 0)
		{
			path_instructions.push_back("STRAIGHT");
		}
		else if ((path_dir[i] - path_dir[i-1] == 1) or (path_dir[i] - path_dir[i-1] == -3))
		{
			path_instructions.push_back("-90");
		}
		else if ((path_dir[i] - path_dir[i-1] == -1) or (path_dir[i] - path_dir[i-1] == 3))
		{
			path_instructions.push_back("+90");
		}
		else
		{
			path_instructions.push_back("GOAL");
		}
	}

	for (unsigned int i = 0; i < path.size(); i++)
	{
		printf("(%2d, %2d)\t[%d]\t[%s]\n", getX(path[i]), getY(path[i]), path_dir[i], path_instructions[i].c_str());
	}

	// print maze with shortest path
	cout << "\n+--";
	for (int r = 1; r < size; r++)
	{
		cout <<"+--";
	}
	cout <<"+"<<endl;

// #define reset	"\x1b[0m"
// #define	red		"\x1b[31m"
// #define green	"\x1b[32m"

	int index = 0;
	while(index<size)
	{
		for(int r = 0; r < size; r++)
		{
			if(maze[(size*index)+r].getWest() == NULL || r == 0)
				cout<<"| \x1b[32m"<<maze[(size*index)+r].getSym() << "\x1b[0m";
			else
			{
				if(maze[(size*index)+r].getWest() != NULL && maze[(size*index)+r].getWestSym() == '|')
					cout<<"  \x1b[32m"<<maze[(size*index)+r].getSym() << "\x1b[0m";
				else
					cout<<"\x1b[32m"<<maze[(size*index)+r].getWestSym()<<" "<<maze[(size*index)+r].getSym() << "\x1b[0m";
			}
		}
		cout<<"|"<<endl;

		if(index == size-1)
		{
			for(int r = 0; r < size; r++)
			{
				if(r == size - 1)
					cout<<"+--";
				else
					cout<<"+--";
			}
			cout<<"+"<<endl;
		}
		else
		{
			for(int r = 0; r < size; r++)
			{
				if(maze[(size*index)+r].getSouth() == NULL)
					cout<<"+--";
				else
				{
					if(maze[(size*index)+r].getSouth() != NULL && maze[(size*index)+r].getSouthSym() == '-')
						cout<<"+  ";
					else
						cout<<"+ \x1b[32m"<<maze[(size*index)+r].getSouthSym() << "\x1b[0m";
				}
			}
			cout <<"+"<<endl;
		}
		index++;
	}
	cout <<endl;

	// Debug, print walls for each point
	// for (int i = 0; i < SIZE*SIZE; i++)
	// {
	// 	printf("Point: (%d %d) \tWalls:\t", getX(i), getY(i));
	// 	if (maze[i].getNorth() == NULL)
	// 		printf("North\t");
	// 	else
	// 		printf("\t");
	// 	if (maze[i].getSouth() == NULL)
	// 		printf("South\t");
	// 	else
	// 		printf("\t");
	// 	if (maze[i].getWest() == NULL)
	// 		printf("West\t");
	// 	else
	// 		printf("\t");
	// 	if (maze[i].getEast() == NULL)
	// 		printf("East\t");
	// 	else
	// 		printf("\t");
	// 	printf("\n");
	// }
}