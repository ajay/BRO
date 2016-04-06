#include <unistd.h>

#include "slam.h"

#define PI 		3.14159

#define ROBOT_RADIUS		7.0

#define NUM_SQUARES			16
#define SQUARE_WIDTH		18.0

#define TICKS_PER_CM  		165.0 / 18.0
#define DEGREES_PER_TICK  	90.0 / 64.0

#define START_X 	SQUARE_WIDTH / 2
#define START_Y     SQUARE_WIDTH / 2
#define START_ANGLE	90

#define LEFT_ENC	0
#define RIGHT_ENC	1

#define TOP_ULTRA		0
#define BOTTOM_ULTRA	1
#define LEFT_ULTRA		2
#define RIGHT_ULTRA		3

#define PING_THRESH 	1.0

float currX;
float currY;

float gridX;
float gridY;

float currAngle;

using namespace std;

void slamSetup() {
	currX = START_X;
	currY = START_Y;

	gridX = 0;
	gridY = 0;

	currAngle = START_ANGLE;
}

float degToRad(float deg) {
	return deg * PI / 180.0;
}

float keepInRange(float num, float lb, float ub) {
	while (num < lb) {
		num += (ub - lb);
	}

	while (num > ub) {
		num -= (ub - lb);
	}

	return num;
}

void sendWallAt(int x1, int y1, int x2, int y2) {
	
}

void findWallWithUltrasonic(uint8_t sensor, float cm) {
  // If the ping hasn't returned, don't do anything
  // NOTE: I don't actually know what returns when nothing returns
  if(cm == 0) {
    return;
  }
  printf("Sensor: %d, Reading: %f\n", sensor, cm);

  /*#define TOP_ULTRA	0
  #define BOTTOM_ULTRA	1
  #define LEFT_ULTRA	2
  #define RIGHT_ULTRA	3*/

  // get angle of ultrasonic sensor
  float sensorAngle = currAngle;
  switch (sensor) {
    case LEFT_ULTRA:
      sensorAngle = keepInRange(sensorAngle + 45, 0, 360);
      break;
    case TOP_ULTRA:
      break;
    case RIGHT_ULTRA:
      sensorAngle = keepInRange(sensorAngle + (360 - 45), 0, 360);
      break;
    case BOTTOM_ULTRA:
      sensorAngle = keepInRange(sensorAngle + 180, 0, 360);
      break;
  }

  // find x and y components of distance where ultrasonic hits
  float radAngle = degToRad(sensorAngle);
  float guessDistX = (currX + (float)(cm + ROBOT_RADIUS) * cos(radAngle)) / SQUARE_WIDTH;
  float guessDistY = (currY + (float)(cm + ROBOT_RADIUS) * sin(radAngle)) / SQUARE_WIDTH;
  printf("Angle: %f \nX: %f Y: %f\n", radAngle, guessDistX, guessDistY);
  int hitWallX = 0, hitWallY = 0;

  // check which wall gets hit
  float wallThresh = (float)PING_THRESH / (float)SQUARE_WIDTH;
  if (floor(guessDistX - wallThresh) != floor(guessDistX + wallThresh)) {
    hitWallX = 1;

    // get the location of the wall
    guessDistX = gridX + round(guessDistX);
    guessDistY = gridY + floor(guessDistY);
  }

  if (floor(guessDistY - wallThresh) != floor(guessDistY + wallThresh)) {
    hitWallY = 1;

    // get the location of the wall
    guessDistX = gridX + floor(guessDistX);
    guessDistY = gridY + round(guessDistY);
  }

  printf("Hit Wall X: %d, Hit Wall Y: %d\n", hitWallX, hitWallY);

  // if it is close enough to a corner, ignore
  // alternatively, if it does not hit anything, ignore
  if (hitWallX == hitWallY) {
    return;
  }

  // if it's on the edge, ignore
  if (hitWallX && (guessDistX == 0 || guessDistX == NUM_SQUARES)) {
    // return;
  }
  
  if (hitWallY && (guessDistY == 0 || guessDistY == NUM_SQUARES)) {
    // return;   
  }

  printf("***************************\n");
  printf("Loc X: %f, Loc Y: %f\n", guessDistX, guessDistY);
  printf("***************************\n");

  // it is now a non-edge wall, send data
  if (hitWallX) {
    if (guessDistX > gridX) {
      sendWallAt(guessDistX - 1, guessDistY, guessDistX, guessDistY);
    } else {
      sendWallAt(guessDistX, guessDistY, guessDistX + 1, guessDistY);
    }
  } else if (hitWallY) {
    if (guessDistY > gridY) {
      sendWallAt(guessDistX, guessDistY - 1, guessDistX, guessDistY);
    } else {
      sendWallAt(guessDistX, guessDistY, guessDistX, guessDistY + 1);
    }  
  }
}

void updateSlam(BruhBot *bruh) {
	//updateLocation(bruh->encoder[LEFT_ENC], bruh->encoder[RIGHT_ENC]);
	//bruh->resetEncoders();
	printf("-----------------------\n");
	printf("Cx: %f Cy: %f\n", currX, currY);
	for (int i = 0; i < 4; i++) {
		findWallWithUltrasonic(i, bruh->ultrasonic[i]);
	}
	printf("-----------------------\n");

	// printf("%f %f %f %f %f\n\r", currX, currY, gridX, gridY, currAngle);
}