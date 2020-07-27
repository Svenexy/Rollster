void moveServo(int servo, int pos, int time) {
  SSCSerial.println("#" + String(servo) + "P" + String(pos) + "T" + String(time));
}

void moveLeg(int servo, int pos, int time) {
  SSCSerial.println("#" + String(servo) + "P" + String(pos) + "T" + String(time));
}
