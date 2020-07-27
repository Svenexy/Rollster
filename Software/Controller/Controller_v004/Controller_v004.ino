#include <Wire.h>
#include <EasyTransfer.h>

#define JOY_BUT1 14

const int MPU_addr = 0x68; // I2C address of the MPU-6050
float AcX, AcY;


//create two objects
EasyTransfer ETin, ETout;



struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t Yas1;
  int16_t Xas1;
  int16_t AcX;
  int16_t AcY;
  bool joyBut1;
};

struct RECEIVE_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t counter;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;


void setup() {

  Serial.begin(115200);
  Serial2.begin(38400);

  ETin.begin(details(rxdata), &Serial2);
  ETout.begin(details(txdata), &Serial2);

  pinMode(JOY_BUT1, INPUT_PULLDOWN);


  // Start Communication with MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);


}


void loop() {

  txdata.joyBut1 = digitalRead(JOY_BUT1);
  txdata.Yas1 = analogRead(A1);
  txdata.Xas1 = analogRead(A2);


  // Read data from MPU6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  txdata.AcX = AcX / 182.04;
  txdata.AcY = AcY / 182.04;

  //then we will go ahead and send that data out
  ETout.sendData();

  //there's a loop here so that we run the recieve function more often then the
  //transmit function. This is important due to the slight differences in
  //the clock speed of different Arduinos. If we didn't do this, messages
  //would build up in the buffer and appear to cause a delay.
  for (int i = 0; i < 5; i++) {
    //remember, you could use an if() here to check for new data, this time it's not needed.
    ETin.receiveData();

    // Counter on Serial
    Serial.print("C = "); Serial.print(rxdata.counter);
    Serial.print("\tYas1 = "); Serial.print(txdata.Yas1);
    Serial.print("\tXas1 = "); Serial.print(txdata.Xas1);
    Serial.print("\tbt1 = "); Serial.print(txdata.joyBut1);
    Serial.print("\t\tAcX = "); Serial.print(txdata.AcX);
    Serial.print("\tAcY = "); Serial.println(txdata.AcY);

    //delay
    delay(10);
  }

  //delay for good measure
  delay(10);
}
