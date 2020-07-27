#ifndef _MORPHEX_Utilities_H
#define _MORPHEX_Utilities_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define PWM_FREQ 50

// Leg dimensions in mm
#define Tibia 91.8
#define Femur 80
#define Coxa 40.24


struct servo {
	uint8_t number;
	uint8_t type;
	int position
};

struct body {
	float x;
	float y;
	float z;
	float facing;
};

struct leg {
	uint8_t number;
	float gamma;
	float alpha;
	float beta;
};

struct arm {
	uint8_t number;
	float a;
	float b;
};




class MORPHEX_Utils {
  public:
    void begin(int offsets);
    void reset(void);

    void leg_calculations (int8_t x, int8_t y, int8_t z);  
    
  private:

  	Servo xServo;
  	Servo yServo;
  	Servo ZServo;

    float RadDeg (float Rad);
    float DegRad (float Deg);
};


#endif
