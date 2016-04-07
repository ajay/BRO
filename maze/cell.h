#include <iostream>

class cell
{
	public:
		cell();
		~cell();

		int north = -1;
		int south = -1;
		int east = -1;
		int west = -1;

		bool visited = false;
};