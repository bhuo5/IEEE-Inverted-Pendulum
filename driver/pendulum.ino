#include "encoder.h"
#include "pid.h"
#include "motor_bts7960.h"

void setup() {
  encoderInit();
  motorInit();
}

void loop() {
  // basic code to test motor driver and encoder at the same time
  Serial.println(encoderGetCount());
  motorSet(encoderGetCount());
  delay(100);
}
