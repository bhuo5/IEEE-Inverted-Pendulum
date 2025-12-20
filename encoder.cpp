#include <Arduino.h>
#include "pins.h"
#include "encoder.h"

// Use volatile because this value is changed inside an interrupt
volatile long encoderCount = 0;

// Interrupt service routine: triggered on channel A change
void encoderISR() {
  // Read channel B to determine rotation direction
  if (digitalRead(ENC_B) == digitalRead(ENC_A)) {
    encoderCount++;  // Forward rotation
  } else {
    encoderCount--;  // Backward rotation
  }
}

// Initialize encoder pins and attach interrupts
void encoderInit() {
  pinMode(ENC_A, INPUT_PULLUP);  // A channel input with pull-up
  pinMode(ENC_B, INPUT_PULLUP);  // B channel input with pull-up

  // Attach interrupt on rising and falling edges for maximum accuracy
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
}
  
// Return current encoder count
long encoderGetCount() {
  return encoderCount;
}
