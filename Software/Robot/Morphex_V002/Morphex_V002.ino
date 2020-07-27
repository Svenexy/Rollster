#include "Morphex_Utils.h"
#include "Filter.h"

#define ledPin 13

bool ledState = LOW;
long previousMillis = 0;
long interval = 1000;

Morphex_Utils Morphex;
                         //1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1 
int offsets[NUM_SERVOS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() 
{ 
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
  delay(1000);
  Morphex.begin(offsets);
  
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
    
//  int16_t xPot = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023)
//  xPot = map(xPot, 0, 1023, 0, 200);
//  int16_t yPot = analogRead(A1);
//  yPot = map(yPot, 0, 1023, 0, 200);
//  int16_t zPot = analogRead(A7);
//  zPot = map(zPot, 0, 1023, 0, 100);
  
  Morphex.stretch();

   
} 
