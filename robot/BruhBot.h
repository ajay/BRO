#ifndef BRUHBOT_H
#define BRUHBOT_H

#include <pthread.h>
#include <vector>
#include <string>

#include "serial.h"

#include <unistd.h>

class BruhBot
{
	public:
		/**
		 * Creates BruhBot object to interact with robot, as well as
		 * connects to robot using connect()
		 */
		BruhBot(void);

		/**
		 * Disconnects BruhBot object from robot
		 */
		~BruhBot(void);

		/**
		 * Queries the number of arduinos connected
		 * @return Number of connected arduinos
		 */
		int numconnected(void);

		/**
		 * Is BruhBot connected to all of it's devices?
		 * @return The status of BruhBot's connection
		 */
		bool connected(void);

		/**
		 * Set's the previous motion back to 0, useful if something goes
		 * wrong or a reset is required for any reason
		 */
		void reset(void);

		/**
		 * Resets values of encoders back to zero, on the arduino side
		 */
		void reset_encoders(void);

		/**
		 * Attemps to connect to all mounted arduinos. Establishes a serial
		 * connection with each one, and differentiates them using each arduino's
		 * DEV_ID.
		 * @return True on a succsessful connection with all devices
		 */
		bool connect(void);

		/**
		 * Disconnects from all connected arduinos
		 */
		void disconnect(void);

		/**
		 * Send strings over serial comm to the ardunios. Manages
		 * the locks required for sending and calls threadSend() to
		 * actually send the serial string
		 * @param motion
		 *        Motion string to send over serial. Current format:
		 *        "[left_motor, right_motor, reset_enc]\n"
		 */
		void send(std::vector<double> motion);

		/**
		 * Used to hold the state of if the encoders
		 * need to be reset on the arduino or not
		 */
		bool reset_enc;

		/**
		 * Used as a sort of "signal handler"
		 * False by default, BruhBot will stop if it is set to true.
		 */
		bool startStop;

		/**
		 * Array that holds the *current* speeds of all motors,
		 * sent from the arduino
		 */
		int motor_speeds[2] = {-1};

		/**
		 * Array that holds the *current* values of all encoders,
		 * send from the arduino
		 */
		long int encoder[2] = {-1};

		/**
		 * Array that holds the *current* values of all ultrasonic sensors in cm,
		 * send from the arduino
		 */
		double ultrasonic[4] = {-1};

		std::string mode = "STOP";
		std::string pid_type = "STOP";
		int pid_info = 0;
		std::vector<int> position;
		int orientation = 0;


		void move(std::string action);

		/**
		 * Flag from Arduino telling pi that the robot can move
		 * to the next step.
		 */
		int pidFinish;

	private:
		/**
		 * Private method for handing data transfer over serial.
		 * Sets up and maintains mutex locks for sending & receiving
		 */
		static void* commHandler(void*);

		/**
		 * Private methods that accepts any data received over serial and
		 * stores it in the correct location. Currently gets info from drive
		 * base, as well as the arm.
		 */
		void threadRecv(void);

		/**
		 * Sends data over serial after being called by send().
		 * Determines which arduino to send data to based on the
		 * DEV_ID of the arduino
		 * @param motion
		 *        The vector to be sent over to the arduinos
		 */
		void threadSend(std::vector<double> motion);

		/**
		 * Manages receiving data over serial. Manages mutex locks
		 * when actually reading the data. This function is only useful
		 * if the actually data received is required. Otherwise, use
		 * threadRecv(), which will actually place the data somewhere instead.
		 * @return  A vector of data received
		 */
		void recv(void);

		/**
		 * Hold the last value that was assigned to the motors over serial.
		 * Used to make sure the same values are not sent over twice.
		 */
		std::vector<double> prev_motion;

		/**
		 * Vector holding values of 255. Used to convert double from
		 * -1 to 1 to a pwm value from -255 to 255. Can be used to limit
		 * overall motion range between any two values.
		 */
		std::vector<double> motion_const;

		/**
		 * Stores current robotid, useful in the future if multiple
		 * BruhBot robots are ever connected
		 */
		int robotid;

		/**
		 * Vector holding all current connections to arduinos
		 */
		std::vector<serial_t *> connections;

		/**
		 * Vector holding DEV_IDs of all current connections
		 */
		std::vector<int> ids;

		/**
		 * Vector holding all pport strings used for arduinos to
		 * connect. Stored with a prefix of '/dev/ttyACM#'
		 */
		std::vector<char *> pports;

		// Threading stuff for handling the communcation
		std::vector<double> commSend;
		std::vector<double> commRecv;
		pthread_t *update_thread;
		pthread_mutex_t *commSendLock;
		pthread_mutex_t *commRecvLock;
};

#endif