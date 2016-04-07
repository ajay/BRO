#include "cell.h"

using namespace std;

cell::cell()
{
	north = NULL;
	south = NULL;
	west = NULL;
	east = NULL;
	visited = 0;
	parent = NULL;
	num = -1;
	sym = ' ';
	southSym = '-';
	westSym = '|';
}

cell::~cell()
{
}

//Mutators
void cell::setNorth(cell* newcell)
{
	north = newcell;
}

void cell::setSouth(cell* newcell)
{
	south = newcell;
}

void cell::setWest(cell* newcell)
{
	west = newcell;
}

void cell::setEast(cell* newcell)
{
	east = newcell;
}

void cell::setWalls(cell *n, cell *s, cell *w, cell*e)
{
	north = n;
	south = s;
	west = w;
	east = e;
}

void cell::setParent(cell* newcell)
{
	parent = newcell;
}

void cell::setVisited(int v)
{
	visited = v;
}

void cell::setNum(int n)
{
	num = n;
}

void cell::setSouthSym(char s)
{
	southSym = s;
}

void cell::setWestSym(char s)
{
	westSym = s;
}

// Accessors
cell* cell::getNorth()
{
	return north;
}

cell* cell::getSouth()
{
	return south;
}

cell* cell::getWest()
{
	return west;
}

cell* cell::getEast()
{
	return east;
}

cell* cell::getParent()
{
	return parent;
}

int cell::getVisited()
{
	return visited;
}

int cell::getNum()
{
	return num;
}

char cell::getSym()
{
	return sym;
}

void cell::setSym(char s)
{
	sym = s;
}

char cell::getSouthSym()
{
	return southSym;
}

char cell::getWestSym()
{
	return westSym;
}