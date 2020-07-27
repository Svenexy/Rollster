void moveServo(int servo, int pos, int time) {
  SSCSerial.println("#" + String(servo) + "P" + String(pos) + "T" + String(time));
}
