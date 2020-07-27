#include <Wire.h>
#include <Servo.h>
#include <EasyTransfer.h>

//create two objects
EasyTransfer ETin, ETout;
//create servo
Servo myservo;

int joystick1 = 0;
int joystick2 = 0;

const int MPU_addr = 0x68; // I2C address of the MPU-6050
float AcX, AcY;


//Universal errornumber to control the led on the teensy
byte ErrorNumber = 0;
//Timer used for the led blinking on the teensy
IntervalTimer Timer_1;


struct RECEIVE_DATA_STRUCTURE {
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

struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t counter;
  int16_t AcX;
  int16_t AcY;
};


//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;


void setup() {
  Serial.begin(115200);
  Serial2.begin(38400);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ETin.begin(details(rxdata), &Serial2);
  ETout.begin(details(txdata), &Serial2);

  pinMode(LED_BUILTIN, OUTPUT);

  myservo.attach(20);


  Timer_1.begin(ErrorBlink, 200000);  // ErrorBlink to run every 200ms
  ErrorNumber = 0;

  setupMPU();

}

void loop() {

  //first, lets read our button and store it in our data structure
  if (txdata.counter > 20000) {
    txdata.counter = 0;
  }

  txdata.counter++;

  // Read data from MPU6050
  readMPU();

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

    joystick1 = firstOrderFilter(rxdata.Yas1, joystick1, 15);
    joystick2 = firstOrderFilter(rxdata.Xas1, joystick2, 15);
    Serial.print("C = "); Serial.print(txdata.counter);
    Serial.print("\tYas1 = "); Serial.print(joystick1);
    Serial.print("\tXas1 = "); Serial.print(joystick2);
    Serial.print("\tbt1 = "); Serial.print(rxdata.joyBut1);
    Serial.print("\t\tAcX = "); Serial.print(rxdata.AcX);
    Serial.print(" \tAcY = "); Serial.println(rxdata.AcY);


    //set our servo position based on what we received from the other Arduino
    //we will also map the ADC value to a servo value
    myservo.write(map(joystick1, 0, 1023, 0, 180));

    //delay
    delay(10);
  }

  //delay for good measure
  delay(10);
}
