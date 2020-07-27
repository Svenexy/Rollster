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
  ETin.begin(details(rxdata), &BLSerial);
  ETout.begin(details(txdata), &BLSerial);

  setupMPU();

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read data from MPU6050
    readMPU();

    txdata.AcX = AcX / 182.04;
    txdata.AcY = AcY / 182.04;

    // If data has been recieved send data back!
    if (ETin.receiveData()) {
      previousSafetyMillis = currentMillis;

      txdata.counter = count;

      ETout.sendData();
    }

    // If no data has been receinced in 200ms this function will start
    else if (currentMillis - previousSafetyMillis > 200) {
      DBGSerial.print("*no data* ");
      rxdata.Yas1 = 512;
      rxdata.Xas1 = 512;
      rxdata.Yas2 = 512;
      rxdata.Xas2 = 512;
    }
    
    // To start add 0 again
    if (count > 20000) {
      count = 0;
    }
    count++;

    DBGSerial.print("C = "); DBGSerial.print(count);
    DBGSerial.print("\tYas1 = "); DBGSerial.print(rxdata.Yas1);
    DBGSerial.print("\tXas1 = "); DBGSerial.print(rxdata.Xas1);
    DBGSerial.print("\tbt1 = "); DBGSerial.println(rxdata.joyBut1);

  }
}
