#include "Morphex_Utils.h"

void Morphex_Utils::begin(int *offsets) {

  for(int i=0; i<NUM_SERVOS; i++) {
    _offsets[i] = *(offsets+i);
  }

  for(int i=0; i<NUM_SERVOS; i++) {
    servos[i].number = i;
    if (i==0 || i==3 || i==6 || i==9 || i==12 || i==15) {
      servos[i].type = TABIA;
    } else if (i==1 || i==4 || i==7 || i==10 || i==13 || i==16) {
      servos[i].type = FEMUR;
    } else if (i==2 || i==5 || i==8 || i==11 || i==14 || i==17) { 
      servos[i].type = COXA;
    } else if ((i % 2 == 0) && i > 17) {
      servos[i].type = ELBOW;
    } else if (i == 30) {
      servos[i].type = WALROLE;
    } else{
      servos[i].type = SHOULDER;
    }
//    Serial.print("\n[INF]\t[SETUP]\tServo #" + (String)i + " has type " + (String)servos[i].type);      
  }
  

  #ifdef DEBUG
    Serial.println("\n[WARN]\t[SETUP]\tDebug mode is active, servos will probably move slowly and time-related functions might not work as intended!");
    Serial.println("\t\tTo disable debug mode, comment out line 12 in Morphex_Utils");
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
  pwm.begin();
  pwm.begin();
  #ifdef DEBUG
    Serial.println("Done!");
    Serial.print("[INF]\t[SETUP]\tSetting PWM frequency to ");
    Serial.print(PWM_FREQ);
    Serial.print(" Hz ... ");
  #endif
  pwm.setPWMFreq(PWM_FREQ);
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
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    pwmove(i, 90+_offsets[i]);
  }

  delay(1000);

  origin.x = 0;
  origin.y = 0;
  origin.z = 0;

  for (int i = 0; i < NUM_LEGS; i++)
  {
    legs[i].number = i;
    legs[i].move = false;
    legs[i].x = 0;
    legs[i].y = 0;
    legs[i].z = 0;
  }
}

void Morphex_Utils::moveServo (uint8_t number, int deg, bool smooth, float speed) {

}

angleLeg* Morphex_Utils::traceLeg (uint8_t leg, float x, float y, float z, int resolution, uint8_t shape) {
  
  if ((legs[leg].x != x) || (legs[leg].y != y) || (legs[leg].z != z)) {
    pointLeg* path = new pointLeg[resolution];
    angleLeg* anglePath = new angleLeg[resolution];

    if (shape == 0) {
      #ifdef DEBUG
        Serial.print("\n[INF]\t[TRACE]\tTracing leg #" + (String)leg + " from [" + (String)legs[leg].x + "; " + (String)legs[leg].y + "; " + (String)legs[leg].z + "] to [" + (String)x + "; " + (String)y + "; " + (String)z + "]");
      #endif
      float stepX = (x-legs[leg].x)/(float)(resolution - 1);
      float stepY = (y-legs[leg].y)/(float)(resolution - 1);
      float stepZ = (z-legs[leg].z)/(float)(resolution - 1);
      Serial.print("\n\t\tStep:\t");
      Serial.print(stepX, 8);
      Serial.print("\t");
      Serial.print(stepY, 8);
      Serial.print("\t");
      Serial.print(stepZ, 8);
      Serial.println();
      for (int i = 0; i < resolution; i++) {
        path[i].x = stepX*(float)i + legs[leg].x;
        path[i].y = stepY*(float)i + legs[leg].y;
        path[i].z = stepZ*(float)i + legs[leg].z;
//        Serial.print("\n\t\tPath:\t" + (String)path[i].x + "\t" + (String)path[i].y + "\t" + (String)path[i].z + "\t");
      }
    }

    legs[leg].x = x;
    legs[leg].y = y;
    legs[leg].z = z;

    #ifdef DEBUG
      Serial.print("\n\t\tTrace parameters:\tL1\tL\tgamma\talpha\tbeta");
    #endif
    
    for (int i=0; i<resolution; i++) {
      float L1 = sqrt(pow(path[i].x,2) + pow(path[i].y,2));
      float L = sqrt(pow(path[i].z,2) + pow((L1 - Coxa),2));
    
      float gamma = atan2(path[i].x,path[i].y);
      anglePath[i].gamma = RadDeg(gamma) + 45 ;
    
      float alpha_1 = acos( path[i].z / L );
      float alpha_2 = acos( (pow(L,2) + pow(Femur,2) - pow(Tibia,2) ) / (2*Femur*L) );
      float alpha = alpha_1 + alpha_2;
      anglePath[i].alpha = RadDeg(alpha);
    
      float beta = acos( (pow(Tibia,2) + pow(Femur,2) - pow(L,2) ) / (2*Tibia*Femur) );
      anglePath[i].beta = RadDeg(beta);
      
      #ifdef DEBUG
        Serial.print("\n\t\t\t\t\t" + (String)L1 + "\t" + (String)L + "\t" + (String)anglePath[i].gamma + "\t" + (String)anglePath[i].alpha + "\t" + (String)anglePath[i].beta);
      #endif
    }

    
    return anglePath;
  } else {
    Serial.print("\n[WARN]\t[TRACE]\t No new path!!");
    angleLeg* anglePath = new angleLeg[0];
    return anglePath;
  }  
}


void Morphex_Utils::setLegs(leg *legs, uint8_t shape) {
  int resolution = 50;
  uint8_t totalMove = 0;
  uint8_t toMove[6] = {255, 255, 255, 255, 255, 255};

  for (int i=0; i<6; i++) {
    if (legs[i].move) {
      toMove[totalMove] = legs[i].number;
      totalMove++;
    }
  }

  angleLeg paths[totalMove][resolution];
  for (int i = 0; i < totalMove; i++) {
    angleLeg* tmp = traceLeg(toMove[i], legs[toMove[i]].x, legs[toMove[i]].y, legs[toMove[i]].z, resolution, shape);
    for (int j = 0; j < resolution; j++) {
      paths[i][j] = *(tmp+j);
    }
    delete[] tmp;
  }
  for (int i=0; i<totalMove; i++) {
    for(int j=0; j<resolution; j++) {
      pwmove(Gamma[toMove[i]], paths[i][j].gamma);
      pwmove(Alpha[toMove[i]], paths[i][j].alpha);
      pwmove(Beta[toMove[i]], paths[i][j].beta);      
    }
  }

  for(int i=0; i<6; i++) {
    legs[i].move = false;
  }
}

void Morphex_Utils::stretch(void) {

  leg legs[6] = {{0, true, 0, 0, 0}, {1, true, 0, 0, 0}, {2, true, 0, 0, 0}, {3, true, 0, 0, 0}, {4, true, 0, 0, 0}, {5, true, 0, 0, 0}};
  
  for(int i=0; i<6; i++) {
    legs[i].move = true;
    legs[i].x = 0; 
    legs[i].y = 0;
    legs[i].z = 0;
  }
  setLegs(legs, 0);
  
//  legs[0] = {0, true, 0, 0, 0};
//  legs[2] = {2, true, 0, 0, 0};
//  legs[4] = {4, true, 0, 0, 0};
//  setLegs(legs, 0);
//  
//  legs[0] = {0, true, 50, 10, 35};
//  legs[2] = {2, true, 50, 10, 35};
//  legs[4] = {4, true, 50, 10, 35};
//  setLegs(legs, 0);
//  
//  legs[1] = {1, true, 0, 0, 0};
//  legs[3] = {3, true, 0, 0, 0};
//  legs[5] = {5, true, 0, 0, 0};
//  setLegs(legs, 0);
  
  legs[1] = {1, true, 50, 50, 35};
  setLegs(legs, 0);
  legs[1] = {1, true, 75, 75, 50};
  setLegs(legs, 0);
  legs[1] = {1, true, 100, 100, 70};
  setLegs(legs, 0);
  legs[1] = {1, true, 100, 100, 100};
  setLegs(legs, 0);
}

void Morphex_Utils::turnLeg(uint8_t leg, float gamma) {



}


int Morphex_Utils::pulseLength(int deg) {
  return map(deg, 0, 180, SERVOMIN, SERVOMAX);
}

void Morphex_Utils::pwmove(uint8_t i, int deg) {
  pwm.setPWM(i, 0, pulseLength(deg+_offsets[i]));
  servos[i].position = deg;
}


float Morphex_Utils::DegRad (float Deg) {
// transform degrees to radians
  return ((Deg*PI)/180);
}

float Morphex_Utils::RadDeg (float Rad) {
// transform radians to degrees
  return ((Rad*180)/PI);
}
