#include "Morphex_Utils.h"
#include <EasyTransfer.h>
#include "Filter.h"

#define ledPin 13


bool ledState = LOW;
long previousMillis = 0;
long interval = 1000;

Morphex_Utils Morphex;
EasyTransfer ETin;
EasyTransfer ETout; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int8_t mode;
  int16_t count;
};

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int8_t mode;
  int16_t x;
  int16_t y;
  int16_t z;
};

RECEIVE_DATA_STRUCTURE rxData;
SEND_DATA_STRUCTURE txData;

                         //1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1 
int offsets[NUM_SERVOS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


void setup() 
{   
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
  Serial2.begin(34800);
  ETin.begin(details(rxData), &Serial2);
  ETout.begin(details(txData), &Serial2);
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
    txData.count = txData.count + 1;
    digitalWrite(ledPin, ledState);
    ETout.sendData();
  }

  if(ETin.receiveData()){
  Serial.print("angleX: ");
  Serial.print(rxData.x);
  Serial.print("\tangleY: ");
  Serial.print(rxData.y);
  Serial.print("\tangleZ: ");
  Serial.println(rxData.z);
  }
  
   
} 
