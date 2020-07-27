#include "Filter.h"

int16_t firstOrderFilter (int16_t inputValue, int16_t currentValue, int16_t filter) {

  int16_t newValue = (inputValue + (currentValue * filter)) / (filter + 1);

  return newValue;
  
}
