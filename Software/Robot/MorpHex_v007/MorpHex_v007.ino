#include <Wire.h>
#include <EasyTransfer.h>

#define DBGSerial Serial
#define BLSerial Serial2
#define SSCSerial Serial3

EasyTransfer ETin, ETout;

int joystick1 = 0;
int joystick2 = 0;

int count = 0;

const int MPU_addr = 0x68;
float AcX, AcY;


//Universal errornumber to control the led on the teensy
byte ErrorNumber = 0;
//Timer used for the led blinking on the teensy
IntervalTimer Timer_1;


struct RECEIVE_DATA_STRUCTURE {
  // THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t Yas1;
  int16_t Xas1;
  int16_t Yas2;
  int16_t Xas2;
  int16_t AcX;
  int16_t AcY;
  bool joyBut1;
};

struct SEND_DATA_STRUCTURE {
  // THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t counter;
  int16_t AcX;
  int16_t AcY;
};


// Give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

unsigned long previousMillis = 0;
const long interval = 40;

long previousSafetyMillis;


void setup() {
  DBGSerial.begin(115200);
  SSCSerial.begin(9600);
  BLSerial.begin(38400);

  // Start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ETin.begin(details(rxdata), &Serial2);
  ETout.begin(details(txdata), &Serial2);

  pinMode(LED_BUILTIN, OUTPUT);


  Timer_1.begin(ErrorBlink, 200000);  // ErrorBlink to run every 200ms
  ErrorNumber = 0;

  setupMPU();

}

void loop() {

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;


    if (ETin.receiveData()) {
      previousSafetyMillis = currentMillis;

      txdata.counter = count;

      ETout.sendData();
    } 
    else if (currentMillis - previousSafetyMillis > 200) {
      DBGSerial.print("*no data* ");
      rxdata.Yas1 = 512;
      rxdata.Xas1 = 512;
      rxdata.Yas2 = 512;
      rxdata.Xas2 = 512;
    }

    if (count > 20000) {
      count = 0;
    }
    count++;

    // Read data from MPU6050
    readMPU();

    txdata.AcX = AcX / 182.04;
    txdata.AcY = AcY / 182.04;

    joystick1 = firstOrderFilter(rxdata.Yas1, joystick1, 15);
    joystick2 = firstOrderFilter(rxdata.Xas1, joystick2, 15);
    Serial.print("C = "); Serial.print(txdata.counter);
    Serial.print("\tYas1 = "); Serial.print(joystick1);
    Serial.print("\tXas1 = "); Serial.print(joystick2);
    Serial.print("\tbt1 = "); Serial.print(rxdata.joyBut1);
    Serial.print("\t\tAcX = "); Serial.print(rxdata.AcX);
    Serial.print(" \tAcY = "); Serial.println(rxdata.AcY);

  }
}
