int16_t firstOrderFilter (int16_t inputValue, int16_t currentValue, int16_t filter) {
  int16_t newValue = (inputValue + (currentValue * filter)) / (filter + 1);
  return newValue;
}



/* ----------------------- PRINT FUNCTIONS ----------------------- */
void printDoubleArray (double array[], int size){
    for(int i = 0; i < size; i++) {
        Serial.print(array[i]);
      	Serial.print("\t");
    }
    Serial.println();
}

void printDoubleArray (int array[], int size){
    for(int i = 0; i < size; i++) {
        Serial.print(array[i]);
      	Serial.print("\t");
    }
    Serial.println();
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
