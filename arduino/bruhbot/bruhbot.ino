/////////////
// BRUHBOT //
/////////////

#include <Encoder.h>
#include <NewPing.h>
#include <string.h>

#include "pid.h"

#define DEV_ID 1
#define RAMP_CONST 1 // Higher is faster
#define SPEED_LIMIT 255
#define BUFSIZE 256

// Motor Pins
#define DRV8835_PHASE_ENABLE_PIN	00
#define MOTOR_LEFT_DIRECTION_PIN	01
#define MOTOR_LEFT_PWM_PIN			03
#define MOTOR_RIGHT_DIRECTION_PIN	02
#define MOTOR_RIGHT_PWM_PIN			04

// Encoder Pins
#define ENCODER_LEFT_GREEN_PIN		16
#define ENCODER_LEFT_YELLOW_PIN		15
#define ENCODER_RIGHT_GREEN_PIN		19
#define ENCODER_RIGHT_YELLOW_PIN	20

// Ultrasonic Pins
#define US_FRONT_TRIGGER_PIN		21
#define US_FRONT_ECHO_PIN			18
#define US_BACK_TRIGGER_PIN			13
#define US_BACK_ECHO_PIN			05
#define US_LEFT_TRIGGER_PIN			23
#define US_LEFT_ECHO_PIN			22
#define US_RIGHT_TRIGGER_PIN		17
#define US_RIGHT_ECHO_PIN			14
#define MAX_DISTANCE 				200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Global vars
const int safesize = BUFSIZE / 2;
 char buf[BUFSIZE];
char msg[BUFSIZE];
char write_buffer[safesize];
unsigned long msecs = 0;
unsigned long timeout = 0;
unsigned int encoder_reset = 0;

// 0 == pid running, not finished
// 1 == pid not running
// 2 == pid running, waiting for pi to respond
unsigned int pidFinish = 1;

// Target and previous velocity arrays
static int target_vel[2];
static int prev_vel[2];

// Initialize ultrasonic sensors
NewPing us_front(US_FRONT_TRIGGER_PIN, US_FRONT_ECHO_PIN, MAX_DISTANCE);
NewPing us_back (US_BACK_TRIGGER_PIN,  US_BACK_ECHO_PIN,  MAX_DISTANCE);
NewPing us_left (US_LEFT_TRIGGER_PIN,  US_LEFT_ECHO_PIN,  MAX_DISTANCE);
NewPing us_right(US_RIGHT_TRIGGER_PIN, US_RIGHT_ECHO_PIN, MAX_DISTANCE);
NewPing ultrasonic[4] = {us_front, us_back, us_left, us_right}; // front, back, left, right
long int ping[4];

// Initialize encoders, left, right
Encoder encoders[2] = {	Encoder(ENCODER_LEFT_GREEN_PIN,  ENCODER_LEFT_YELLOW_PIN),
						Encoder(ENCODER_RIGHT_GREEN_PIN, ENCODER_RIGHT_YELLOW_PIN)};

#define TICKS_90 150
#define TICKS_1_TILE 300
#define TICK_THRESH 10

// Limit function
int limit(int x, int a, int b)
{
	if 		(x > b) { return b; }
	else if (x < a)	{ return a;	}
	else			{ return x;	}
}

void assignLeft(int speed, int direction)
{
	analogWrite(MOTOR_LEFT_PWM_PIN, speed);
	if 		(direction == 1)	{ digitalWrite(MOTOR_LEFT_DIRECTION_PIN, HIGH);	 } // Go backwards
	else if (direction == 0)	{ digitalWrite(MOTOR_LEFT_DIRECTION_PIN, LOW);	 } // Go forwards
}

void assignRight(int speed, int direction)
{
	analogWrite(MOTOR_RIGHT_PWM_PIN, speed);
	if 		(direction == 1)	{ digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, HIGH); } // Go backwards
	else if (direction == 0)	{ digitalWrite(MOTOR_RIGHT_DIRECTION_PIN, LOW);	 } // Go forwards
}

void setmotors(int left_speed, int right_speed)
{
	// Array for forwards/backwards directions (1 = backwards)
	bool isneg[2] = {left_speed < 0, right_speed < 0};

	// Limit motor speeds to range (0 to 255)
	left_speed = limit(abs(left_speed), 0, 255);
	right_speed = limit(abs(right_speed), 0, 255);

	// Assign motor values
	assignLeft(left_speed, isneg[0]);
	assignRight(right_speed, isneg[1]);
}

int rampMotor(int current_vel, int target_vel)
{
	int delta_vel = target_vel - current_vel;
	delta_vel = limit(delta_vel, -RAMP_CONST, RAMP_CONST);
	current_vel = limit(current_vel + delta_vel, -SPEED_LIMIT, SPEED_LIMIT);
	return current_vel;
}

int absAverageEnc() {
	return (abs(encoders[0].read()) + abs(encoders[1].read())) / 2;
}

int errorStraight() {
	return encoders[0].read() - encoders[1].read();
}

int errorForward() {
	return TICKS_1_TILE - averageEnc();
}

int errorTurn() {
	return TICKS_90 - absAverageEnc();
}

int averageEnc() {
	return (encoders[0].read() + encoders[1].read()) / 2;
}

bool finishForward(int encVal) {
	return abs(encVal - TICKS_1_TILE) <= TICK_THRESH;
}

bool finishTurn(int encVal) {
	return abs(encVal - TICKS_90) <= TICK_THRESH;
}

// 2 = STRAIGHT, 3 = TURN_+90, 4 = TURN_-90
void pidWithDir(int dir) {
	if (pidFinish) {
		return;
	}

	static PID PID_forward = PID(0.75, 0.00035, 0.0, &errorForward, &averageEnc, &finishForward);
	static PID PID_turn = PID(2.0, 0.0005, 0, &errorTurn, &absAverageEnc, &finishTurn);
	static PID PID_straight = PID(2.81, 0.002, 0, &errorStraight, &errorStraight, NULL);

	int leftSpeed = 0, rightSpeed = 0;
	switch (dir) {
		case 2:
			leftSpeed = PID_forward.getCurrSpeed();// - PID_straight.getCurrSpeed();
			rightSpeed = PID_forward.getCurrSpeed();// + PID_straight.getCurrSpeed();

			break;
		case 3:
			leftSpeed = -PID_turn.getCurrSpeed();
			rightSpeed = PID_turn.getCurrSpeed();

			break;
		case 4:
			leftSpeed = PID_turn.getCurrSpeed();
			rightSpeed = -PID_turn.getCurrSpeed();

			break;
		default:
			Serial.println("lol");
			break;
	}

	setmotors(leftSpeed, rightSpeed);
	if (leftSpeed == 0 && rightSpeed == 0) {
		pidFinish = 2;
		PID_forward.reset();
		PID_straight.reset();
		PID_turn.reset();
	}
}

void setup()
{
	// Begin serial
	Serial.begin(57600);

	// Set output pins
	pinMode(MOTOR_LEFT_DIRECTION_PIN,	OUTPUT);  // APHASE  --> Left Motor Direction (0 or 1)
	pinMode(MOTOR_LEFT_PWM_PIN,			OUTPUT);  // AENBL   --> Left Motor Speed (0-255)
	pinMode(MOTOR_RIGHT_DIRECTION_PIN,	OUTPUT);  // BPHASE  --> Right Motor Direction (0 or 1)
	pinMode(MOTOR_RIGHT_PWM_PIN,			OUTPUT);  // BENBL   --> Right Motor Speed (0-255)
	pinMode(DRV8835_PHASE_ENABLE_PIN,	OUTPUT);  // MODE    --> HIGH for PHASE/ENABLE
	digitalWrite(DRV8835_PHASE_ENABLE_PIN, HIGH); // MODE    --> Setting MODE pin to HIGH

	timeout = millis();
}

void loop()
{
	int available_bytes = 0;
	if ((available_bytes = Serial.available()))
	{
		// Read + attach null byte to read string
		int obytes = strlen(buf);
		Serial.readBytes(&buf[obytes], available_bytes);
		buf[available_bytes + obytes] = '\0';

		// Resize read string just in case
		if (strlen(buf) > safesize)
		{
			memmove(buf, &buf[strlen(buf) - safesize], safesize);
			buf[safesize] = '\0';
		}

		char *s, *e;

		if ((e = strchr(buf, '\n')))
		{
			e[0] = '\0';
			if ((s = strrchr(buf, '[')))
			{
				// Parse string being read
				// Left motor, right motor, reset encoders
				sscanf(s, "[%d %d %d]\n", &target_vel[0], &target_vel[1], &encoder_reset);
				timeout = millis();
			}
			memmove(buf, &e[1], strlen(&e[1]) + sizeof(char));
		}
	}

	// if it's not pid, act normal
	if (encoder_reset < 2) {
		if (pidFinish == 2) {
			pidFinish = 1;
		}

		if (encoder_reset)
		{
			encoders[0].write(0);
			encoders[1].write(0);
			encoder_reset = 0;
		}

		// EMERGENCY STOP: MASTER COMM LOST
		if (millis() - timeout > 500)
		{
			// After .5 seconds, stop the robot
			memset(target_vel, 0, sizeof(int) * 2);
			memset(prev_vel, 0, sizeof(int) * 2);
			setmotors(0, 0);
			timeout = millis();
		}

		// Ramp motors values, and determine next value to set
		for (int i = 0; i < 2; i++)
		{
			prev_vel[i] = rampMotor(prev_vel[i], target_vel[i]);
		}

		setmotors(prev_vel[0], prev_vel[1]);
	} else {
		if (pidFinish == 1) {
			pidFinish = 0;
		}
		// do pid
		pidWithDir(encoder_reset);
   if (pidFinish == 2) {
    setmotors(0, 0); 
   }
	}

	// Send back data over serial every 100ms
	if (millis() - msecs > 50)
	{
		for (int i = 0; i < 4; i++)
		{
			ping[i] = 1000 * ultrasonic[i].ping_cm();
		}

		sprintf(write_buffer, "[%d %d %d %ld %ld %ld %ld %ld %ld %d]\n",
				DEV_ID,
				prev_vel[0],
				prev_vel[1],
				ping[0],
				ping[1],
				ping[2],
				ping[3],
				encoders[0].read(),
				encoders[1].read(),
				pidFinish);
		Serial.print(write_buffer);
		msecs = millis();
	}
}

