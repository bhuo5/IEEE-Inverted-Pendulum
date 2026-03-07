#include <Arduino.h>

// Encoder pins (OMRON E6B2)
#define ENC_A 0  // Encoder channel A
#define ENC_B 1  // Encoder channel B

#define PPR 1024.0 // pulses per rotation
#define PI 3.14159
#define VELOCITY_DELAY 10

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

float encoderGetAngle() {
  return (2 * PI * encoderCount / (2 * PPR)); // Divide by 2 * PPR since we are incrementing on both rising and falling edges
}

float encoderGetAngularVelocity() {
  float theta1 = encoderGetAngle();
  delay(VELOCITY_DELAY);
  float theta2 = encoderGetAngle();

  return (1000 * (theta2 - theta1) / VELOCITY_DELAY);
}
