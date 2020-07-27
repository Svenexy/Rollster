#include "EasyTransfer.h"

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  boolean blinks = false;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop(){

  mydata.blinks = true;
  ET.sendData();
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(1000);
  
  mydata.blinks = false;
  ET.sendData();
  digitalWrite(LED_BUILTIN, LOW); 

  delay(1000);
}



