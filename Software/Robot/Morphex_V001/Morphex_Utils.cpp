#include "Morphex_Utils.h"

Morphex_Utils::Morphex_Utils() {
  
}

void Morphex_Utils::begin(int offsets) {


  #ifdef DEBUG
    Serial.println("\n[WARN]\t[SETUP]\tDebug mode is active, servos will probably move slowly and time-related functions might not work as intended!");
    Serial.println("\t\tTo disable debug mode, comment out line 12 in AP_Utils.h");
    Serial.print("\t\tResuming setup in ");
    for(int i=10; i>0; i--) {
      Serial.print(i);
      delay(250);
      for(int j=0; j<3; j++) {
        Serial.print('.');
        delay(250);
      }
    }
    Serial.print("\n[INF]\t[SETUP]\tStarting PWM ... ");
  #endif
  xServo.attach(17);
  yServo.attach(18);
  zServo.attach(19);
  #ifdef DEBUG
    Serial.println("Done!");
    Serial.print("[INF]\t[SETUP]\tSetting all servos to default position ... ");
  #endif
  reset();
  #ifdef DEBUG
    Serial.println("Done!");
    Serial.println("[INF]\t[SETUP]\tSuccesfully started!");
  #endif
  
}

void Morphex_Utils::reset() {


}

void Morphex_Utils::leg_calculations (int16_t x, int16_t y, int16_t z) {

  float L1 = sqrt(pow(x,2) + pow(y,2));
  float L = sqrt(pow(z,2) + pow((L1 - Coxa),2));

  float gamma = atan2(x,y);
  gamma = RadDeg(gamma) + 45 ;

  float alpha_1 = acos( (z) / (L) );
  alpha_1 = RadDeg(alpha_1);
  float alpha_2 = acos( (pow(L,2) + pow(Femur,2) - pow(Tibia,2) ) / (2*Femur*L) );
  alpha_2 = RadDeg(alpha_2);
  float alpha = alpha_1 + alpha_2;

  float beta = acos( (pow(Tibia,2) + pow(Femur,2) - pow(L,2) ) / (2*Tibia*Femur) );
  beta = (beta*180)/PI;
  Xservo.write((int)gamma);
  Yservo.write((int)alpha);
  Zservo.write((int)beta);
  
  #ifdef DEBUG
    Serial.print(x);
    Serial.print("  ,  ");
    Serial.print(y);
    Serial.print("  ,  ");
    Serial.print(z);
    Serial.print("  |   |  ");
    Serial.print(L1);
    Serial.print("  ,  ");
    Serial.print(L);
    Serial.print("  |  ");
    Serial.print(alpha_1);
    Serial.print("  ,  ");
    Serial.print(alpha_2);
    Serial.print("  |   |  ");
    Serial.print(gamma);
    Serial.print("  ,  ");
    Serial.print(alpha);
    Serial.print("  ,  ");
    Serial.println(beta);
  #endif
  
}

float DegRad (float Deg) {
// transform degrees to radians
  return ((Deg*PI)/180);
}

float RadDeg (float Rad) {
// transform radians to degrees
  return ((Rad*180)/PI);
}
