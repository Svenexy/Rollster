#ifndef _MORPHEX_Utilities_H
#define _MORPHEX_Utilities_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_PWMServoDriver.h>

//#define DEBUG

// PCA9650 specifications
#define PWM_FREQ 50
#define NUM_SERVOS 18
#define NUM_LEGS 6

// Directions
#define FORWARD   0
#define BACKWARD  1
#define LEFT      2
#define RIGHT     3

// Units
#define M  1
#define CM 100
#define MM 1000

// Servo specifications
#define SERVOMIN    150
#define SERVOMAX    560

// LEGS
#define TABIA 0
#define FEMUR 1
#define COXA  2

// ARM
#define SHOULDER 3
#define ELBOW 4

// Servo for walking or roling
#define WALROLE 5

// Maximum and minimum of the robot
#define HORIZ_MAX   130
#define HORIZ_DEF   90
#define HORIZ_MIN   50
#define VERT_MAX    130 
#define VERT_DEF    90
#define VERT_MIN    50

// Leg dimensions in mm
#define Tibia 91.8
#define Femur 80
#define Coxa 40.24


struct servo {
	uint8_t number;
	uint8_t type;
	int position;
};

struct body {
	float x;
	float y;
	float z;
	float facing;
};

struct pointLeg {
	float x;
	float y;
	float z;
};

struct angleLeg {
  float gamma;
  float alpha;
  float beta;
};

struct leg {
	uint8_t number;
	bool move;
	float x;
	float y;
	float z;
};

struct arm {
	uint8_t number;
	float a;
	float b;
};




class Morphex_Utils {
  public:
    void begin(int *offsets);
    void reset(void);

    void moveServo (uint8_t number, int deg, bool smooth, float speed);
    angleLeg* traceLeg (uint8_t leg, float x, float y, float z, int resolution, uint8_t shape);
    void setLegs(leg *legs, uint8_t shape);  
    void turnLeg (uint8_t leg, float gamma);

    void stretch(void);
    
  private:
  	int _offsets[NUM_SERVOS];
  	servo servos[NUM_SERVOS];
  	body origin;
  	leg legs[NUM_LEGS];
    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
    uint8_t Gamma[6] = {0, 3, 6, 9, 12, 15};
    uint8_t Alpha[6] = {1, 4, 7, 10, 13, 16};
    uint8_t Beta[6] = {2, 5, 8, 11, 14, 17};
    
  	void pwmove(uint8_t i, int deg);
  	int pulseLength(int deg);

    float RadDeg (float Rad);
    float DegRad (float Deg);
};


#endif
