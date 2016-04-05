//Generates maze using dfs
//output: 1's are walls. S = start and E = end

#include <iostream>
#include <ctime>
#include <termios.h>
#include <stack>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


using namespace std;

/********************************
* USED FOR THE SIZE OF THE MAZE *
*   Must be an odd number for   *
*    maze traversal reasons     *
********************************/
#define SIZE 11

static struct termios old, new1;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new1 = old; /* make new settings same as old settings */
  new1.c_lflag &= ~ICANON; /* disable buffered i/o */
  new1.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new1); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

// CELL STRUCTURE
struct Cell
{
	bool visited;
	bool top_wall;
	bool bot_wall;
	bool left_wall;
	bool right_wall;
	char display;
};

// FUNCTION DECLARATIONS
void Initialize(Cell Level[][SIZE]);
void ClearScreen();
void Redraw(Cell Level[][SIZE]);
void GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY);
void SaveMaze(Cell Level[][SIZE]);

// MAIN
int main() {
	Cell Level[SIZE][SIZE];
	int posX = 0;
	int posY = 0;
	int goalX = 0;
	int goalY = 0;
	bool game_over = false;

	while(!game_over) {
		system("clear");
		Initialize(Level);
		Redraw(Level);
		GenerateMaze(Level, posX, posY, goalX, goalY);
		SaveMaze(Level);

		char input;
		cout << "Create a new Maze? (Y)/(N): ";
		cin >> input;

		if((input != 'n') && (input != 'N') && (input != 'y') && (input != 'Y'))
			cout << "Invalid option" << endl;
		else if((input == 'n') || (input == 'N')) {
			game_over = true;
			cout << "Good bye!" << endl;
		}
	}

	getche();
	return 0;
}

// INITIALIZE MAZE
void Initialize(Cell Level[][SIZE]) {
	for(int i=0; i<SIZE; i++) {
		for(int j=0; j<SIZE; j++) {
			Level[i][j].display = '*';
			Level[i][j].visited = false;
			Level[i][j].top_wall = true;
			Level[i][j].bot_wall = true;
			Level[i][j].left_wall = true;
			Level[i][j].right_wall = true;
		}
	}
	for(int i=1; i<SIZE-1; i++) {
		for(int j=1; j<SIZE-1; j++) {
			// Border Cells have fewer accessible walls
			Level[1][j].top_wall = false;
			Level[SIZE-2][j].bot_wall = false;
			Level[i][1].left_wall = false;
			Level[i][SIZE-2].right_wall = false;
		}
	}
}

// CLEAR SCREEN
void ClearScreen()
{
	cout << "\f";
}

// REDRAW MAZE
void Redraw(Cell Level[][SIZE]) {
	for(int i=0; i<SIZE; i++) {
		cout << endl;
		for(int j=0; j< SIZE; j++)
			cout << " " << Level[i][j].display;
	}
}

// GENERATE MAZE
void GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY) {
	srand((unsigned)time(NULL));				        					// Pick random start cell
	int random = 0;
	int randomX = ((2*rand())+1)%(SIZE-1);						// Generate a random odd number between 1 and SIZE
	int randomY = ((2*rand())+1)%(SIZE-1);						// Generate a random odd number between 1 and SIZE
	posX = randomX;                       						// Save player's initial X position
	posY = randomY;                      						// Save player's initial Y position
	int visitedCells = 1;
	int totalCells = ((SIZE-1)/2)*((SIZE-1)/2);
	int percent = 0;
	stack<int> back_trackX, back_trackY; 						// Stack is used to trace the reverse path

	Level[randomY][randomX].display = 'S';						// Set S as the start cell
	Level[randomY][randomX].visited = true;						// Set start cell as visited;

	while(visitedCells < totalCells)
	{
		if(((Level[randomY-2][randomX].visited == false) && (Level[randomY][randomX].top_wall == true && Level[randomY-2][randomX].bot_wall == true)) ||
		   ((Level[randomY+2][randomX].visited == false) && (Level[randomY][randomX].bot_wall == true && Level[randomY+2][randomX].top_wall == true)) ||
		   ((Level[randomY][randomX-2].visited == false) && (Level[randomY][randomX].left_wall == true && Level[randomY][randomX-2].right_wall == true)) ||
		   ((Level[randomY][randomX+2].visited == false) && (Level[randomY][randomX].right_wall == true && Level[randomY][randomX+2].left_wall == true)))
		{
			random = (rand() % 4) + 1;		// Pick a random wall 1-4 to knock down

			 // GO UP
			if((random == 1) && (randomY > 1)) {
				if(Level[randomY-2][randomX].visited == false) {	// If not visited
					Level[randomY-1][randomX].display = ' ';	// Delete display
					Level[randomY-1][randomX].visited = true;	// Mark cell as visited
					Level[randomY][randomX].top_wall = false;	// Knock down wall

					back_trackX.push(randomX); 			// Push X for back track
					back_trackY.push(randomY);			// Push Y for back track

					randomY -= 2;					// Move to next cell
					Level[randomY][randomX].visited = true;		// Mark cell moved to as visited
					Level[randomY][randomX].display = ' ';		// Update path
					Level[randomY][randomX].bot_wall = false;	// Knock down wall
					visitedCells++;					// Increase visitedCells counter
				}
				else
					continue;
			}

			// GO DOWN
			else if((random == 2) && (randomY < SIZE-2)) {
				if(Level[randomY+2][randomX].visited == false) {	// If not visited
					Level[randomY+1][randomX].display = ' ';	// Delete display
					Level[randomY+1][randomX].visited = true;	// Mark cell as visited
					Level[randomY][randomX].bot_wall = false;	// Knock down wall

					back_trackX.push(randomX); 			// Push X for back track
					back_trackY.push(randomY);			// Push Y for back track

					randomY += 2;					// Move to next cell
					Level[randomY][randomX].visited = true;		// Mark cell moved to as visited
					Level[randomY][randomX].display = ' ';		// Update path
					Level[randomY][randomX].top_wall = false;	// Knock down wall
					visitedCells++;					// Increase visitedCells counter
				}
				else
					continue;
			}

			// GO LEFT
			else if((random == 3) && (randomX > 1)) {
				if(Level[randomY][randomX-2].visited == false) {	// If not visited
					Level[randomY][randomX-1].display = ' ';	// Delete display
					Level[randomY][randomX-1].visited = true;	// Mark cell as visited
					Level[randomY][randomX].left_wall = false;	// Knock down wall

					back_trackX.push(randomX); 			// Push X for back track
					back_trackY.push(randomY);			// Push Y for back track

					randomX -= 2;					// Move to next cell
					Level[randomY][randomX].visited = true;		// Mark cell moved to as visited
					Level[randomY][randomX].display = ' ';		// Update path
					Level[randomY][randomX].right_wall = false;	// Knock down wall
					visitedCells++;					// Increase visitedCells counter
				}
				else
					continue;
			}

			// GO RIGHT
			else if((random == 4) && (randomX < SIZE-2)) {
				if(Level[randomY][randomX+2].visited == false) {	// If not visited
					Level[randomY][randomX+1].display = ' ';	// Delete display
					Level[randomY][randomX+1].visited = true;	// Mark cell as visited
					Level[randomY][randomX].right_wall = false;	// Knock down wall

					back_trackX.push(randomX); 			// Push X for back track
					back_trackY.push(randomY);			// Push Y for back track

					randomX += 2;					// Move to next cell
					Level[randomY][randomX].visited = true;		// Mark cell moved to as visited
					Level[randomY][randomX].display = ' ';		// Update path
					Level[randomY][randomX].left_wall = false;	// Knock down wall
					visitedCells++;					// Increase visitedCells counter
				}
				else
					continue;
			}

			percent = (visitedCells*100/totalCells*100)/100;		// Progress in percentage
			cout << endl << "	Generating a Random Maze... " << percent << "%" << endl;
		}
		else {
			randomX = back_trackX.top();
			back_trackX.pop();

			randomY = back_trackY.top();
			back_trackY.pop();
		}

		ClearScreen();
		Redraw(Level);
	}

	goalX = randomX;
	goalY = randomY;
	Level[goalY][goalX].display = 'E';
	system("clear");
	ClearScreen();
	Redraw(Level);
	cout << endl << "\a\t	Complete!" << endl;
}

// SAVE MAZE
void SaveMaze(Cell Level[][SIZE]) {
	ofstream output;
	char file[20];
	char input;

	cout << endl << "Save Maze? (Y)/(N): ";
	cin >> input;

	if ((input == 'y') || (input == 'Y')) {
		cout << endl << "Save as: ";
		cin >> file;

		output.open(file);

		for (int i = 0; i < SIZE; i++) {
			output << endl;
			for (int j = 0; j < SIZE; j++) {
				output << Level[i][j].display << " ";
			}
		}

		cout << "Maze has been saved to" << "/" << file << "/" << endl;
		output.close();
	}

}