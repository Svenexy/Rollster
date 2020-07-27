#include "EasyTransfer.h"

//create object
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  boolean blinks = false;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

   //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(mydata), &Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { // run over and over
  //check and see if a data packet has come in. 
  if(ET.receiveData()){

    if(mydata.blinks == true) {
      
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);  
    }
    
  }
}
