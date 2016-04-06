#include "pid.h"

double k_p = 0.005;
double k_i = 0.001;
double k_d = 0.001;

void pid_straight(int ticks);
{
	while (1)
	{
		while (pid_kill)
		{
			sleep(0.1);
		}

		double speed = 0.9;
		rose.reset_encoders();
		int average = 0;

		arma::vec error = zeros<vec>(2);
		arma::vec error_sum = zeros<vec>(2);
		arma::vec prev_error = zeros<vec>(2);
		arma::vec error_diff = zeros<vec>(2);

		while (!pid_kill)
		{
			// Average is now the master
			int error = bruh.

			if (average >= 360)
			{
				pid_kill = true;
			}

			for (unsigned int i = 0; i < 4; i++)
			{
				error[i] = average - rose.encoder[i];
				error_sum[i] += error[i];
				error_diff[i] = error[i] - prev_error[i];
				prev_error[i] = error[i];
			}

			// Slaves
			motion[0] = speed + (error[1] * k_p) + (error_sum[1] * k_i) + (error_diff[1] * k_d);
			motion[1] = speed + (error[1] * k_p) + (error_sum[1] * k_i) + (error_diff[1] * k_d);
			motion[2] = speed + (error[2] * k_p) + (error_sum[2] * k_i) + (error_diff[2] * k_d);
			motion[3] = speed + (error[3] * k_p) + (error_sum[3] * k_i) + (error_diff[3] * k_d);

			printf("average: [%d]\n", average);
			printf("rose_encoder: [%d %d %d %d]\n", (int)rose.encoder[0], (int)rose.encoder[1], (int)rose.encoder[2], (int)rose.encoder[3]);
			printf("error: [%d %d %d %d]\n", (int)error[0], (int)error[1], (int)error[2], (int)error[3]);
			printf("error_sum: [%d %d %d %d]\n", (int)error_sum[0], (int)error_sum[1], (int)error_sum[2], (int)error_sum[3]);
			printf("error_diff: [%d %d %d %d]\n", (int)error_diff[0], (int)error_diff[1], (int)error_diff[2], (int)error_diff[3]);
			printf("motion: [%2.5f %2.5f %2.5f %2.5f]\n\n", motion[0], motion[1], motion[2], motion[3]);

			rose.send(motion);
			usleep(100000); // 100ms
		}
	}
}