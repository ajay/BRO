#include <iomanip>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <sstream>

#include "BruhBot.h"
// #include "slam.h"
#include "pid.h"
#include "window.h"
#include "maze.h"

using namespace std;

static BruhBot bruh;
Maze* maze = new Maze();
static vector<double> motion = {0, 0};
static int stopsig;

void auton()
{
	// usleep(2000000);
	while (1)
	{
		while (bruh.mode != "AUTO")
		{
			usleep(100000); // 100ms
		}


		printf("starting mapping\n");

		maze->map(&bruh);
		// for (unsigned int i = 0; i < maze->path.size(); i++)
		// {
		// 	// if (bruh.mode != "AUTO")
		// 	// {
		// 	// 	cout << bruh.mode << "\n";
		// 	// 	break;
		// 	// }

		// 	usleep(500000); // 0.5 s
		// 	bruh.pid_type = "STOP";
		// 	// printf("---State %d of %d---\n", i, (int)maze->path.size()-1);
		// 	// printf("Target: (%2d, %2d)\n", maze->getX(maze->path[maze->path.size()-1]), maze->getY(maze->path[maze->path.size()-1]));
		// 	// printf("Current location: (%2d, %2d)\n", maze->getX(maze->path[i]), maze->getY(maze->path[i]));
		// 	// printf("Current action: %s\n", maze->path_instructions[i].c_str());

		// 	if (maze->path_instructions[i] == "GOAL")
		// 	{
		// 		bruh.mode = "STOP";
		// 		break;
		// 	}
		// 	else
		// 	{
		// 		bruh.reset_encoders();
		// 		bruh.mode = "PID";
		// 		// printf("Starting pid\n");

		// 		bruh.pid_type = maze->path_instructions[i];
		// 		/*if ((maze->path_instructions[i] == "TURN_-90") or (maze->path_instructions[i] == "TURN_+90"))
		// 		{
		// 			usleep(500000); // 500ms
		// 			bruh.reset_encoders();
		// 			usleep(500000); // 500ms
		// 			bruh.pid_type = "STRAIGHT";
		// 		}*/
		// 	}

			while (bruh.pid_type != "ADVANCE")
			{
				//cout << bruh.pid_type << "\n";
				usleep(10000); // 10ms
			}

			bruh.mode = "AUTO";
		// }
	}
}

// Takes in two integers and assignments them to motion
void drive(double left_speed, double right_speed)
{
	motion[0] = left_speed;
	motion[1] = right_speed;
}

void drive(string direction, double v)
{
	if		(direction == "STOP")				{ drive( 0,  0); }

	else if	(direction == "FORWARD")			{ drive( v,  v); }
	else if (direction == "BACKWARD")			{ drive(-v, -v); }
	else if (direction == "LEFT")				{ drive(-v,  v); }
	else if (direction == "RIGHT")				{ drive( v, -v); }

	else if	(direction == "FORWARD_LEFT")		{ drive( v/2,  v); }
	else if	(direction == "FORWARD_RIGHT")		{ drive( v,  v/2); }
	else if (direction == "BACKWARD_LEFT")		{ drive(-v/2, -v); }
	else if (direction == "BACKWARD_RIGHT")		{ drive(-v, -v/2); }

	else if (direction == "MOTOR_LEFT")			{ drive( v,  0); }
	else if (direction == "MOTOR_RIGHT")		{ drive( 0,  v); }

	else if (direction == "STOP")				{ drive( 0,  0); }
}

void stop(int signo)
{
	printf("Exiting yo >>>>\n");
	bruh.startStop = true;
	bruh.disconnect();
	stopsig = 1;
	exit(1);
}

void print_SDL(std::ostringstream& str, int size, int x, int y)
{
	SDL_Renderer* renderer = get_renderer();
	SDL_Color color = { 255, 255, 255, 255 };
	std::string font = "fonts/Anonymous.ttf";

	std::string str_string = str.str();
	SDL_Texture *str_image = renderText(str_string, font, color, size, renderer);
	renderTexture(str_image, renderer, x, y);
}

int main(int argc, char *argv[])
{
	argv[1] = "fdfs";

	bruh.startStop = false;
	signal(SIGINT, stop);
	bool quit = false;

	thread pid(pid_straight, &bruh);
	thread autonmous(auton);

	SDL_Surface *screen = initSDL();
	SDL_Event event;
	SDL_Window* window = get_window();
	SDL_Renderer* renderer = get_renderer();
	SDL_Texture* texture = get_texture();

	double v = 0.5; // velocity
	string direction = "STOP";

	//maze->createPreviousBrownMaze();
	maze->createMiniMaze();
	maze->DFS();

	while(!quit)
	{
		std::ostringstream speed, mode_string, direction_string, motor_speeds, encoders, us[4];
		speed << "Speed: " << std::setprecision(2) << v;
		mode_string << "Mode: " << bruh.mode;
		direction_string << "Direction: " << direction;
		motor_speeds << "Motor Speeds: [" << bruh.motor_speeds[0] << " " << bruh.motor_speeds[1] << "]";
		encoders << "Encoders: [" << bruh.encoder[0] << " " << bruh.encoder[1] << "]";

		SDL_RenderClear(renderer);
		print_SDL(speed, 20, 10, 10);
		print_SDL(mode_string, 20, 10, 50);
		print_SDL(direction_string, 20, 10, 90);
		print_SDL(motor_speeds, 20, 10, 130);

		string us_string[] = {"Front", "Back ", "Left ", "Right"};
		for (int i = 0; i < 4; i++)
		{
			us[i] << "Ultrasonic " << i << " (" << us_string[i] <<  "): " << std::setprecision(4) << bruh.ultrasonic[i] << " cm";
			print_SDL(us[i], 20, 10, 170 + 30*i);
		}

		print_SDL(encoders, 20, 10, 300);

		SDL_RenderPresent(renderer);

		SDL_PollEvent(&event);
		const Uint8 *keystates = SDL_GetKeyboardState(NULL);

		if (event.type == SDL_KEYDOWN)
		{
			if (keystates[SDL_SCANCODE_A] && (v < 1))
			{
				v += 0.05;
			}
			else if(keystates[SDL_SCANCODE_S] && (v > -1))
			{
				v -= 0.05;
			}
		}

		if 		(keystates[SDL_SCANCODE_R]) 												{ bruh.reset_encoders(); }
		else if (keystates[SDL_SCANCODE_U])													{ bruh.mode = "DRIVE"; }
		else if (keystates[SDL_SCANCODE_I] || argv[1] != NULL)								{ bruh.mode = "AUTO"; }
		else if (keystates[SDL_SCANCODE_O])													{ bruh.mode = "PID"; }
		else if (keystates[SDL_SCANCODE_P])													{ bruh.mode = "STOP"; }

		if (bruh.mode == "DRIVE")
		{
			if 		((keystates[SDL_SCANCODE_UP]) and (keystates[SDL_SCANCODE_LEFT]))		{ direction = "FORWARD_LEFT"; }
			else if ((keystates[SDL_SCANCODE_UP]) and (keystates[SDL_SCANCODE_RIGHT]))		{ direction = "FORWARD_RIGHT"; }
			else if ((keystates[SDL_SCANCODE_DOWN]) and (keystates[SDL_SCANCODE_LEFT]))		{ direction = "BACKWARD_LEFT"; }
			else if ((keystates[SDL_SCANCODE_DOWN]) and (keystates[SDL_SCANCODE_RIGHT]))	{ direction = "BACKWARD_RIGHT"; }
			else if (keystates[SDL_SCANCODE_UP])											{ direction = "FORWARD"; }
			else if (keystates[SDL_SCANCODE_DOWN])											{ direction = "BACKWARD"; }
			else if (keystates[SDL_SCANCODE_LEFT])											{ direction = "LEFT"; }
			else if (keystates[SDL_SCANCODE_RIGHT]) 										{ direction = "RIGHT"; }
			else if (keystates[SDL_SCANCODE_1]) 											{ direction = "MOTOR_LEFT"; }
			else if (keystates[SDL_SCANCODE_2]) 											{ direction = "MOTOR_RIGHT"; }

			else																			{ direction = "STOP"; }

			drive(direction, v);
			bruh.send(motion);
		}

		else if (bruh.mode == "PID")
		{
			bruh.pid_info = 1000;

			if 		(keystates[SDL_SCANCODE_UP])											{ bruh.pid_type = "STRAIGHT"; }
			else if (keystates[SDL_SCANCODE_LEFT])											{ bruh.pid_type = "TURN_+90"; }
			else if (keystates[SDL_SCANCODE_RIGHT]) 										{ bruh.pid_type = "TURN_-90"; }
		}

		else if (bruh.mode == "STOP")
		{
			bruh.pid_type = "STOP";
			direction = "STOP";
			// bruh.reset_encoders();
			bruh.send(vector<double>({0, 0}));
		}

		if(keystates[SDL_SCANCODE_Q])
		{
			quit = true;
		}
	}
	SDL_Quit();
	stop(0);
}