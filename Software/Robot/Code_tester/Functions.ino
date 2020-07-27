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
