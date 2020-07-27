#define DBGSerial Serial
#define BLSerial Serial2
#define SSCSerial Serial3


void setup() {
  DBGSerial.begin(115200);
  SSCSerial.begin(9600);
  BLSerial.begin(38400);
  



  moveServo(0, 1500, 2000);
  moveServo(1, 1500, 2000);
  moveServo(2, 1500, 2000);
  
}

void loop() {
  delay(2000);
  moveServo(0, 700, 1500);
  delay(2000);
  moveServo(0, 2400, 1500);  
}
