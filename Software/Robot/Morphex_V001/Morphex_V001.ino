#include "Morphex_Utils.h"
#include "Filter.h"

#define ledPin 13
 
Servo Xservo;  // create servo object to control a servo 
Servo Yservo;  // create servo object to control a servo 
Servo Zservo;  // create servo object to control a servo 

bool ledState = LOW;
long previousMillis = 0;
long interval = 1000;

MORPHEX_Utils Morphex;

int offsets[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 
void setup() 
{ 
  pinMode(ledPin, OUTPUT);

  Xservo.attach(17);
  Yservo.attach(18);
  Zservo.attach(19);
  
  Serial.begin(9600);
  Morphex.begin(0);
} 
 
void loop() 
{ 
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
      
    digitalWrite(ledPin, ledState);
  }
    
  int16_t xPot = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023)
  xPot = map(xPot, 0, 1023, 0, 200);
  int16_t yPot = analogRead(A1);
  yPot = map(yPot, 0, 1023, 0, 200);
  int16_t zPot = analogRead(A7);
  zPot = map(zPot, 0, 1023, 0, 100);

//  Xservo.write((int)xPot);
//  Yservo.write((int)yPot);
//  Zservo.write((int)zPot);
  
  Morphex.leg_calculations(xPot, xPot, zPot);
  delay(20);
   
} 
