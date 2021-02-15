#include <Wire.h>
#include <EasyTransfer.h>

#define JOY_BUT1 20

#define DBGSerial Serial
#define BLSerial Serial2
#define NexSerial Serial3

const int MPU_addr = 0x68; // I2C address of the MPU-6050
float AcX, AcY;

int counter = 0;


//create two objects
EasyTransfer ETin, ETout;


struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t Yas1;
  int16_t Xas1;
  int16_t Yas2;
  int16_t Xas2;
  int16_t AcX;
  int16_t AcY;
  bool joyBut1;
};

struct RECEIVE_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t counter;
  int16_t AcX;
  int16_t AcY;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

unsigned long previousMillis = 0;
const long interval = 40;

long previousSafetyMillis;

void setup() {

  DBGSerial.begin(9600);
  BLSerial.begin(38400);
  NexSerial.begin(38400);

  delay(2000);

  nextionStatusText(0);

  ETin.begin(details(rxdata), &BLSerial);
  ETout.begin(details(txdata), &BLSerial);

  pinMode(JOY_BUT1, INPUT_PULLDOWN);

  setupMPU();
  DBGSerial.println("Setup Complete!");


}


void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {  // start timed event for read and send
    previousMillis = currentMillis;

    txdata.joyBut1 = digitalRead(JOY_BUT1);
    txdata.Yas1 = analogRead(A0);
    txdata.Xas1 = analogRead(A1);
    txdata.Yas2 = analogRead(A2);
    txdata.Xas2 = analogRead(A3);

    // Read data from MPU6050
    readMPU();

    txdata.AcX = AcX / 182.04;
    txdata.AcY = AcY / 182.04;

    nextionSetNumber("yas1", txdata.Yas1);
    nextionSetNumber("xas1", txdata.Xas1);
    nextionSetNumber("yas2", txdata.Yas2);
    nextionSetNumber("xas2", txdata.Xas2);
    nextionSetNumber("yas3", txdata.AcX);
    nextionSetNumber("xas3", txdata.AcY);

    ETout.sendData();
    if (ETin.receiveData()) {

      previousSafetyMillis = currentMillis;

      // Send data to Nextion display
      nextionCounter(rxdata.counter);
      nextionSetNumber("yas4", rxdata.AcX);
      nextionSetNumber("xas4", rxdata.AcY);

      
    }

    // If no data has been receinced in 200ms this function will start
    else if (currentMillis - previousSafetyMillis > 200) {
      DBGSerial.print("*no data* ");
      nextionStatusText(0);
    }

    else {
      counter = rxdata.counter;
      nextionStatusText(1);
    }


    DBGSerial.print("C = "); DBGSerial.println(rxdata.counter);
  }

}
