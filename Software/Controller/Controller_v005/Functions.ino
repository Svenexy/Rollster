// ErrorBlink() gets called every 200ms 'timer-interrupt'
void ErrorBlink() {
  static uint16_t LedSequence_normal = 0b0000111100001111;
  static uint16_t LedSequence_1      = 0b0000000000000001;
  static uint16_t LedSequence_2      = 0b0000000000000101;
  static uint16_t LedSequence_3      = 0b0000000000010101;
  static uint16_t LedSequence_4      = 0b0000000001010101;
  static uint16_t LedSequence_5      = 0b0000000101010101;
  static uint16_t LedSequence_6      = 0b0000010101010101;
  static uint16_t LedSequence_7      = 0b0101010101010101;
  static uint16_t LedSequence_8      = 0b0000000111101101;
  static uint16_t Mask = 0;

  Mask <<= 1;
  if (Mask == 0) {
    Mask = 0x0001;
  }

  switch (ErrorNumber)
  {
    case 0:  digitalWrite(LED_BUILTIN, (LedSequence_normal & Mask) > 0); break;// 50% blink
    case 1:  digitalWrite(LED_BUILTIN, (LedSequence_1 & Mask) > 0); break;     // 1 flits / sec
    case 2:  digitalWrite(LED_BUILTIN, (LedSequence_2 & Mask) > 0); break;     // 2 flits / sec
    case 3:  digitalWrite(LED_BUILTIN, (LedSequence_3 & Mask) > 0); break;     // 3 flits / sec
    case 4:  digitalWrite(LED_BUILTIN, (LedSequence_4 & Mask) > 0); break;     // 4 flits / sec
    case 5:  digitalWrite(LED_BUILTIN, (LedSequence_5 & Mask) > 0); break;     // 5 flits / sec
    case 6:  digitalWrite(LED_BUILTIN, (LedSequence_6 & Mask) > 0); break;     // 6 flits / sec
    case 7:  digitalWrite(LED_BUILTIN, (LedSequence_7 & Mask) > 0); break;     // 2.5 Hz
    default: digitalWrite(LED_BUILTIN, (LedSequence_8 & Mask) > 0); break;     // Special
  }
}


int16_t firstOrderFilter (int16_t inputValue, int16_t currentValue, int16_t filter) {
  int16_t newValue = (inputValue + (currentValue * filter)) / (filter + 1);
  return newValue;
}

void readMPU() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
}

void setupMPU() {
  // Start Communication with MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
