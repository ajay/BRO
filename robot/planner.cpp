#include <iomanip>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>

#include "BruhBot.h"
// #include "slam.h"
#include "window.h"

using namespace arma;
using namespace std;

static BruhBot bruh;
static vec motion = zeros<vec>(2);
static int stopsig;
static bool pid_kill = true;
static string mode = "STOP";

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
	bruh.startStop = false;
	signal(SIGINT, stop);
	bool quit = false;

	SDL_Surface *screen = initSDL();
	SDL_Event event;
	SDL_Window* window = get_window();
	SDL_Renderer* renderer = get_renderer();
	SDL_Texture* texture = get_texture();

	double v = 0.5; // velocity
	string direction = "STOP";

	// slamSetup();

	while(!quit)
	{
		std::ostringstream speed, mode_string, direction_string, motor_speeds, encoders, us[4];
		speed << "Speed: " << std::setprecision(2) << v;
		mode_string << "Mode: " << mode;
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
		else if (keystates[SDL_SCANCODE_I])													{ mode = "DRIVE"; }
		else if (keystates[SDL_SCANCODE_O])													{ mode = "PID"; }
		else if (keystates[SDL_SCANCODE_P])													{ mode = "STOP"; }

		if (mode == "DRIVE")
		{
			pid_kill = true;

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

		else if (mode == "PID")
		{
			pid_kill = false;
		}

		else if (mode == "STOP")
		{
			pid_kill = true;
			direction = "STOP";
			bruh.send(zeros<vec>(2));
		}

		if(keystates[SDL_SCANCODE_Q])
		{
			quit = true;
		}
	}
	SDL_Quit();
	stop(0);
}