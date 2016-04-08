#include <cstring>
#include <dirent.h>
#include <pthread.h>
#include <termios.h>
#include <string>

#include "BruhBot.h"

#define WBUFSIZE 128
#define DEV_BAUD  B57600
#define SYNC_NSEC 100000000

// using namespace arma;
using namespace std;

// Limit the value of x between min and max (min < x < max)
static double limitf(double x, double min, double max)
{
	if 		(x < min) 	{ return min; }
	else if (x > max) 	{ return max; }
	else 				{ return x;	  }
}

BruhBot::BruhBot(void)
{
	this->prev_motion = vector<double>(2, 0);
	this->motion_const = vector<double>(2, 255);
	if (this->connect())
	{
		this->reset();
		this->send(vector<double>(2, 0));
	}
	else
	{
		if (this->numconnected() <= 0)
		{
			printf("[BruhBot] Could not connect to any arduinos...\n");
		}

		else
		{
			printf("[BruhBot] Connecting to a particular arduino failed\n");
		}

		exit(0);
	}
	
	this->pidFinish = 1;
}

BruhBot::~BruhBot(void)
{
	if (this->connected())
	{
		this->send(vector<double>(2, 0));
		this->reset();
		this->disconnect();
	}
}

int BruhBot::numconnected(void)
{
	return this->connections.size();
}

bool BruhBot::connected(void)
{
	return this->connections.size() > 0;
}

void BruhBot::reset(void)
{
	this->prev_motion = vector<double>(2, 0);
}

void BruhBot::reset_encoders(void)
{
	this->reset_enc = true;
}

void* BruhBot::commHandler(void* args)
{
	BruhBot *bruh = (BruhBot*) args;

	while (!(bruh->startStop))
	{
		vector<double> tempSendVec;
		pthread_mutex_lock(bruh->commSendLock);
		tempSendVec = bruh->commSend;
		pthread_mutex_unlock(bruh->commSendLock);
		bruh->threadSend(tempSendVec);

		bruh->threadRecv();
	}

	return NULL;
}

bool BruhBot::connect(void)
{
	// Open directory to look for arduinos
	DIR *device_dir = opendir("/dev/");
	struct dirent *entry;

	// Iterate through all possible mounted arduinos
	while ((entry = readdir(device_dir)))
	{
		if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
		{
			if (strstr(entry->d_name, "ttyACM"))
			{
				char *pport = new char[strlen("/dev/") + strlen(entry->d_name) + 1];
				sprintf(pport, "/dev/%s", entry->d_name);
				this->pports.push_back(pport);
			}
		}
	}

	// Close directy that was being traversed
	closedir(device_dir);

	// If file is empty, then disconnect
	if (this->pports.size() == 0)
	{
		this->disconnect();
		return false;
	}

	// Create delay to read at correct rate
	struct timespec synctime;
	synctime.tv_nsec = SYNC_NSEC % 1000000000;
	synctime.tv_sec = SYNC_NSEC / 1000000000;

	// Attempt to connect to all arduinos
	for (char *pport : this->pports)
	{
		// Connect Device
		serial_t *connection = new serial_t;
		serial_connect(connection, pport, DEV_BAUD);

		if (!connection->connected)
		{
			continue;
		}
		else
		{
			this->connections.push_back(connection);
		}
	}

	// Read a message from each device
	nanosleep(&synctime, NULL);
	char *msg = (char*)"";
	for (serial_t *connection : this->connections)
	{
		do
		{
			msg = serial_read(connection);
		} while (!msg || strlen(msg) == 0);
	}

	// Read another one in case that one was garbage
	nanosleep(&synctime, NULL);
	for (size_t i = 0; i < this->connections.size(); i++)
	{
		serial_t *connection = this->connections[i];

		// Read message from arduino
		do
		{
			msg = serial_read(connection);
		} while (!msg || strlen(msg) == 0);

		// If a valid device, add as connected, otherwise disconnect
		int id;
		sscanf(msg, "[%d ", &id);

		// Make sure DEV_ID is not less than 1
		if (id > 0)
		{
			this->ids.push_back(id);
		}
		else
		{
			serial_disconnect(connection);
			this->connections.erase(this->connections.begin() + i);
			delete connection;
		}
	}

	// Disconnect if number of devices is not enough, or there are too many
	if (!this->connected())
	{
		printf("[BruhBot] Stuck checking for connected()\n");
		this->disconnect();
		return false;
	}

	else if (this->numconnected() == 0)
	{
		printf("[BruhBot] Could not connect to all arduinos\n");
		return false;
	}

	else
	{
		printf("[BruhBot] Connected to all\n");

		this->reset_enc = true;

		// Create thread locks and threads
		this->update_thread = new pthread_t;
		this->commSendLock = new pthread_mutex_t;
		this->commRecvLock = new pthread_mutex_t;
		pthread_mutex_init(this->commSendLock, NULL);
		pthread_mutex_init(this->commRecvLock, NULL);

		// Start the update thread
		pthread_create(this->update_thread, NULL, this->commHandler, this);

		return true;
	}
}

void BruhBot::disconnect(void)
{
	if (this->connections.size() > 0)
	{
		for (serial_t *connection : this->connections)
		{
			serial_disconnect(connection);
			delete connection;
		}

		this->connections.clear();
		this->ids.clear();
	}

	if (this->pports.size() > 0)
	{
		for (char *pport : this->pports)
		{
			delete pport;
		}

		this->pports.clear();
	}
	this->robotid = 0;
}

void BruhBot::send(std::vector<double> motion)
{
	// Lock the data before setting it...avoids the thread from reading the motion vector before it finishes copying over
	if (this->numconnected() > 0)
	{
		pthread_mutex_lock(this->commSendLock);
		this->commSend = motion;
		pthread_mutex_unlock(this->commSendLock);
	}
}

void BruhBot::threadSend(std::vector<double> motion)
{
	vector<double> new_motion = motion;

	// Safety check
	if (new_motion.size() != motion_const.size())
	{
		new_motion = vector<double>(motion_const.size(), 0);
	}

	// Boundary check
	for (int i = 0; i < (int)new_motion.size(); i++)
	{
		new_motion[i] = limitf(new_motion[i], -1.0, 1.0);
		new_motion[i] *= motion_const[i];
	}

	char msg[WBUFSIZE];
	for (int i = 0; i < (int)this->connections.size(); i++)
	{
		int runMode;
		switch (this->ids[i])
		{
			// Arduino #1: BruhBot
			case 1:

				runMode = this->reset_enc;
				if (runMode == 0) {
					if (!this->mode.compare(0, 3, "PID")) {
						if (!this->pid_type.compare(0, 8, "STRAIGHT")) {
							runMode = 2;
						} else if (!this->pid_type.compare(0, 8, "TURN_+90")) {
							runMode = 3;
						} else if (!this->pid_type.compare(0, 8, "TURN_-90")) {
							runMode = 4;
						}
					}
				}
				printf("Run: %d Pid finish: %d\n", runMode, pidFinish);

				this->prev_motion[0] = new_motion[0];
				this->prev_motion[1] = new_motion[1];
					sprintf(msg, "[%d %d %d]\n",
						(int)new_motion[0],
						(int)new_motion[1],
						runMode);

				serial_write(this->connections[i], msg);

				if (this->reset_enc)
				{
					this->reset_enc = false;
				}

				break;

			default:
				break;
		}
	}
}

void BruhBot::recv(void)
{
	// Add a lock to wait until the commthread is done setting the vector
	vector<double> tempVec;
	pthread_mutex_lock(this->commRecvLock);
	tempVec = this->commRecv;
	pthread_mutex_unlock(this->commRecvLock);
	// return tempVec;
}

void BruhBot::threadRecv(void)
{
	// Create delay to read at correct rate
	struct timespec synctime;
	synctime.tv_nsec = SYNC_NSEC % 1000000000;
	synctime.tv_sec = SYNC_NSEC / 1000000000;
	nanosleep(&synctime, NULL);

	for (int i = 0; i < (int)this->connections.size(); i++)
	{
		char* msg = serial_read(this->connections[i]);

		if (msg != NULL)
		{
			// printf("[BruhBot] RECEIVED: %s\n", msg);
		}

		switch (this->ids[i])
		{
			case 1: // Arduino #1: BruhBot
				if (msg != NULL)
				{
					double ultrasonic_1000[4];
					// printf("%s\n", msg);

					sscanf(msg, "[%*d %d %d %lf %lf %lf %lf %ld %ld %d]\n",
						&this->motor_speeds[0],
						&this->motor_speeds[1],
						&ultrasonic_1000[0],
						&ultrasonic_1000[1],
						&ultrasonic_1000[2],
						&ultrasonic_1000[3],
						&this->encoder[0],
						&this->encoder[1],
						&this->pidFinish);

					for (int i = 0; i < 4; i++)
					{
						this->ultrasonic[i] = (float)((float)ultrasonic_1000[i] / 1000);
					}
				}

			default:
				break;
		}

		if (this->pidFinish == 2) {
			printf("Done.\n");
			if (this->mode == "AUTO")
			{
				this->pid_type = "ADVANCE";
			}
			else
			{
				// DEBUG: this->pid_type = "STOP";
				// current
				this->pid_type = "ADVANCE";
			}
			this->pidFinish = 1;
		}
	}
}