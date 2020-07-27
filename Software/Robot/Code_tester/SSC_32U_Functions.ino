void moveServo(int servo, int pos, int time) {
//  pos = double(pos+90)/180*1800+600;
  DBGSerial.println("#" + String(servo) + "P" + String(pos) + "T" + String(time));
  SSCSerial.println("#" + String(servo) + "P" + String(pos) + "T" + String(time));
}

void moveLeg(int servo, int pos, int time) {
  SSCSerial.println("#" + String(servo) + "P" + String(pos) + "T" + String(time));
}
