#include <Arduino.h>
#include "pid.h"

#define NUM_FINISH 25

PID::PID(float kP, float kI, float kD, 
         int (*errorFunc)( void ), int (*sensorFunc)( void ), bool (*finishFunc)( int )) 
{
  this->kP = kP;
  this->kI = kI;
  this->kD = kD;

  this->error_sum = 0;
  this->prev_error = 0;
  this->error_diff = 0;

  this->base_vel = 0;

  this->finishCount = 0;

  this->errorFunc = errorFunc;
  this->sensorFunc = sensorFunc;
  this->finishFunc = finishFunc;
}

void PID::reset() {
  this->error_sum = 0;
  this->prev_error = 0;
  this->error_diff = 0;

  this->base_vel = 0;

  this->finishCount = 0;
}

void PID::setBaseVel(double new_vel) {
	this->base_vel = new_vel;
}

// Limit function
float PID::limit(float x, float a, float b)
{
	if 		(x > b) { return b; }
	else if (x < a)	{ return a;	}
	else			{ return x;	}
}

int PID::getCurrSpeed() {
	int sensorVal = this->sensorFunc();
	int error = this->errorFunc();

	if (finishFunc != NULL) {
		bool isFinished = this->finishFunc(sensorVal);
    finishCount = (isFinished ? finishCount + 1 : 0);
		if (isFinished && finishCount >= NUM_FINISH) {
			return 0;
		}
	}

	this->error_sum += error;
	this->error_diff = error - this->prev_error;

	int spd = (int)
	(
		kP * error + 
		kI * this->error_sum +
		kD * this->error_diff +
		255 * this->base_vel
	);

	char debug[50];
 	sprintf(debug, "%f %d %f %f %f %d %d", base_vel, error, error_sum, prev_error, error_diff, spd, sensorVal);
 	Serial.println(debug);

	this->prev_error = error;
	return limit(spd, -255, 255);
}
