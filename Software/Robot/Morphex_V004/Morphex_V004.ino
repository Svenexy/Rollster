#include "Morphex_Utils.h"

Morphex_Utils Morphex;

int offsets[NUM_SERVOS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {

  Serial.begin(115200);

  Morphex.begin(offsets);

}

void loop() {

  Morphex.stretch();
  delay(1000);

}
