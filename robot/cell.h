#include <iostream>

class cell
{
	public:
		cell();
		~cell();

		//Mutators
		void setNorth(cell*);
		void setSouth(cell*);
		void setWest(cell*);
		void setEast(cell*);
		void setWalls(cell *n, cell *s, cell *w, cell*e);
		void setParent(cell*);
		void setVisited(int);
		void setNum(int);
		void setSym(char);
		void setSouthSym(char);
		void setWestSym(char);

		//Accessors
		cell* getNorth();
		cell* getSouth();
		cell* getWest();
		cell* getEast();
		cell* getParent();
		int getVisited();
		int getNum();
		char getSym();
		char getSouthSym();
		char getWestSym();

	protected:
		//Member items
		cell *north;
		cell *south;
		cell *west;
		cell *east;
		int visited;
		cell *parent;
		int num;
		char sym;
		char southSym;
		char westSym;
};