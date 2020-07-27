#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050_tockn.h>
#include "EasyTransfer.h"
#include <Wire.h>

#define RX 10
#define TX 11
#define ledPin 13

LiquidCrystal_I2C lcd(0x27,20,4);
SoftwareSerial BTserial(RX, TX);
EasyTransfer ETin;
EasyTransfer ETout; 
MPU6050 mpu(Wire);

ISR (TIMER1_OVF_vect) {
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
    TCNT1 = 3036;
}

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int8_t mode;
  int16_t count;
};

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int8_t mode;
  int16_t x;
  int16_t y;
  int16_t z;
};

RECEIVE_DATA_STRUCTURE rxData;
SEND_DATA_STRUCTURE txData;

void setup() {

  pinMode(ledPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting up Controller!");

  Serial.begin(115200);
  BTserial.begin(34800);
  ETin.begin(details(rxData), &BTserial);
  ETout.begin(details(txData), &BTserial);
  
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
  
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 3036;

  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1);
}

void loop() {
  
  mpu.update();
  txData.x = mpu.getAngleX();
  txData.y = mpu.getAngleY();
  txData.z = mpu.getAngleZ();
  ETout.sendData();

  if(ETin.receiveData()){
    lcd.setCursor(0,3);
    lcd.print(rxData.count);
    lcd.setCursor(8,3);
    lcd.print(rxData.mode);
  }
  
  lcd.setCursor(0,0);
  lcd.print("angleX: ");
  lcd.setCursor(8,0);
  lcd.print(txData.x);
  lcd.setCursor(0,1);
  lcd.print("angleY: ");
  lcd.setCursor(8,1);
  lcd.print(txData.y);
  lcd.setCursor(0,2);
  lcd.print("angleZ: ");
  lcd.setCursor(8,2);
  lcd.print(txData.z);
}
