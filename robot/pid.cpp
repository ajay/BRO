#include <unistd.h>

#include "pid.h"

#define TICKS_90 150
#define TICKS_1_TILE 300

double k_p = 0.0005;
double k_i = 0.00001;
double k_d = 0.0001;

void pid_straight(BruhBot *bruh)
{
	while (1)
	{
		/*while ((bruh->mode != "PID") and (bruh->mode != "AUTO"))
		{
			usleep(100000); // 100ms
		}

		double base_vel = 0.35;
		bruh->reset_encoders();
		usleep(100000); // 100ms

		int error = 0;
		int error_sum = 0;
		int prev_error = 0;
		int error_diff = 0;
		int average = 0;
		std::vector<double> motion = {0, 0};

		while (((bruh->mode == "PID") and (bruh->pid_type == "STRAIGHT") and (average < TICKS_1_TILE)) or
			   ((bruh->mode == "AUTO") and (bruh->pid_type == "STRAIGHT") and (average < TICKS_1_TILE)))
		{
			average = (int)((bruh->encoder[0] + bruh->encoder[1]) / 2);

			base_vel = 0.35 - (average / 3000);

			// conventinal pid error: left - right
			int error = bruh->encoder[0] - bruh->encoder[1];
			error_sum += error;
			error_diff = error - prev_error;
			prev_error = error;

			motion[0] = base_vel - k_p * error
								 - k_i * error_sum
								 - k_d * error_diff;

			motion[1] = base_vel + k_p * error
								 + k_i * error_sum
								 + k_d * error_diff;

			// printf("error: %d\n", error);
			// printf("error_sum: %d\n", error_sum);
			// printf("error_diff: %d\n", error_diff);
			// printf("motion: [%2.5f %2.5f]\n\n", motion[0], motion[1]);

			bruh->send(motion);

			usleep(10000); // 10ms
		}


		while (((bruh->mode == "PID") and (bruh->pid_type == "TURN_+90") and (average < TICKS_90)) or
			   ((bruh->mode == "AUTO") and (bruh->pid_type == "TURN_+90") and (average < TICKS_90)))
		{
			average = ((abs(bruh->encoder[0]) + abs(bruh->encoder[1])) / 2);

			base_vel = 0.25;

			// conventinal pid error: left - right
			int error = average - TICKS_90;
			error_sum += error;
			error_diff = error - prev_error;
			prev_error = error;

			motion[0] = -base_vel// - k_p * error
								 // - k_i * error_sum
								 - k_d * error_diff;

			motion[1] = base_vel// + k_p * error
								//+ k_i * error_sum
								 + k_d * error_diff;

			// printf("error: %d\n", error);
			// printf("error_sum: %d\n", error_sum);
			// printf("error_diff: %d\n", error_diff);
			// printf("motion: [%2.5f %2.5f]\n\n", motion[0], motion[1]);

			bruh->send(motion);

			usleep(10000); // 10ms
		}

		while (((bruh->mode == "PID") and (bruh->pid_type == "TURN_-90") and (average < TICKS_90)) or
			   ((bruh->mode == "AUTO") and (bruh->pid_type == "TURN_-90") and (average < TICKS_90)))
		{
			average = ((abs(bruh->encoder[0]) + abs(bruh->encoder[1])) / 2);

			base_vel = 0.25;

			// conventinal pid error: left - right
			int error = average - TICKS_90;
			error_sum += error;
			error_diff = error - prev_error;
			prev_error = error;

			motion[0] = base_vel// - k_p * error
								 // - k_i * error_sum
								 + k_d * error_diff;

			motion[1] = -base_vel// + k_p * error
								//+ k_i * error_sum
								 - k_d * error_diff;

			// printf("error: %d\n", error);
			// printf("error_sum: %d\n", error_sum);
			// printf("error_diff: %d\n", error_diff);
			// printf("motion: [%2.5f %2.5f]\n\n", motion[0], motion[1]);

			bruh->send(motion);

			usleep(10000); // 10ms
		}*/


		if (bruh->pidFinish == 2) {
			if (bruh->mode == "AUTO")
			{
				bruh->pid_type = "ADVANCE";
			}
			else
			{
				bruh->pid_type = "STOP";
			}
			bruh->pidFinish = 1;
		}
	}



}

// void pid_straight(BruhBot *bruh)
// {
// 	while (1)
// 	{
// 		while (bruh->mode != "PID")
// 		{
// 			usleep(100000); // 100ms
// 		}

// 		double base_vel = 0.25;
// 		bruh->reset_encoders();

// 		int error = 0;
// 		int error_sum = 0;
// 		int prev_error = 0;
// 		int error_diff = 0;
// 		int average = 0;
// 		std::vector<double> motion = {0, 0};

// 		while ((bruh->mode == "PID") and (average < bruh->pid_info))
// 		{
// 			average = (int)((bruh->encoder[0] + bruh->encoder[1]) / 2);
// 			// conventinal pid error: left - right
// 			int error = bruh->encoder[0] - bruh->encoder[1];
// 			error_sum += error;
// 			error_diff = error - prev_error;
// 			prev_error = error;

// 			motion[0] = base_vel - k_p * error
// 								 - k_i * error_sum
// 								 - k_d * error_diff;

// 			motion[1] = base_vel + k_p * error
// 								 + k_i * error_sum
// 								 + k_d * error_diff;

// 			printf("error: %d\n", error);
// 			printf("error_sum: %d\n", error_sum);
// 			printf("error_diff: %d\n", error_diff);
// 			printf("motion: [%2.5f %2.5f]\n\n", motion[0], motion[1]);

// 			bruh->send(motion);

// 			usleep(100000); // 100ms
// 		}

// 		bruh->mode = "STOP";
// 	}
// }

void turn(int degrees)
{
}