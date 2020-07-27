#include <Wire.h>
#include <EasyTransfer.h>

#define JOY_BUT1 20

const int MPU_addr = 0x68; // I2C address of the MPU-6050
float AcX, AcY;

int counter = 0;


//create two objects
EasyTransfer ETin, ETout;

//Universal errornumber to control the led on the teensy
byte ErrorNumber = 0;
//Timer used for the led blinking on the teensy
IntervalTimer Timer_1;



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

unsigned long previousDispMillis = 0;
const long Dispinterval = 10;


void setup() {

  Serial.begin(9600);
  Serial2.begin(38400);
  Serial3.begin(38400);

  delay(2000);

  nextionStatusText(0);

  ETin.begin(details(rxdata), &Serial2);
  ETout.begin(details(txdata), &Serial2);

  pinMode(JOY_BUT1, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);

  Timer_1.begin(ErrorBlink, 200000);  // ErrorBlink to run every 200ms
  ErrorNumber = 0;

  setupMPU();


}


void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {  // start timed event for read and send
    previousMillis = currentMillis;

    if (counter != rxdata.counter) {
      counter = rxdata.counter;
      ErrorNumber = 0;
      nextionStatusText(1);
    } else {
      ErrorNumber = 7;
      nextionStatusText(0);
    }

    txdata.joyBut1 = digitalRead(JOY_BUT1);
    txdata.Yas1 = analogRead(A0);
    txdata.Xas1 = analogRead(A1);
    txdata.Yas2 = analogRead(A2);
    txdata.Xas2 = analogRead(A3);

    // Read data from MPU6050
    readMPU();

    txdata.AcX = AcX / 182.04;
    txdata.AcY = AcY / 182.04;

    ETout.sendData();

  }

  if (currentMillis - previousDispMillis >= Dispinterval) {  // start timed event for read and send
    previousDispMillis = currentMillis;

    if (ETin.receiveData()) {
      // Send data to Nextion display
      nextionCounter(rxdata.counter);
      nextionSetNumber("yas1", txdata.Yas1);
      nextionSetNumber("xas1", txdata.Xas1);
      nextionSetNumber("yas2", txdata.Yas2);
      nextionSetNumber("xas2", txdata.Xas2);
      nextionSetNumber("yas3", txdata.AcX);
      nextionSetNumber("xas3", txdata.AcY);
      nextionSetNumber("yas4", rxdata.AcX);
      nextionSetNumber("xas4", rxdata.AcY);
    }
  }
}