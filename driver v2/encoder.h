#ifndef encoder_h // include guard
#define encoder_h

#include <Arduino.h>

#define PPR 1024.0 // pulses per rotation
#define PI 3.14159
#define VELOCITY_DELAY_MS 5  // 5 ms delay between angle measurements to calculate angular velocity

// Use volatile because this value is changed inside an interrupt
volatile long count = 0;

// Encoder pins (OMRON E6B2)
int enc_A = 0;  // Encoder channel A
int enc_B = 0;  // Encoder channel B

// Interrupt service routine: triggered on channel A change
void encoder_ISR() {
  // Read channel B to determine rotation direction
  if (digitalRead(enc_B) == digitalRead(enc_A)) {
    count++;  // Forward rotation
  } else {
    count--;  // Backward rotation
  }
}

// Initialize encoder pins and attach interrupts
void encoder_init(int chA, int chB) {
  enc_A = chA;  // assign global variables with "constructor" parameters
  enc_B = chB;

  pinMode(enc_A, INPUT_PULLUP);  // A channel input with pull-up
  pinMode(enc_B, INPUT_PULLUP);  // B channel input with pull-up

  // Attach interrupt on rising and falling edges for maximum accuracy
  attachInterrupt(digitalPinToInterrupt(enc_A), encoder_ISR, CHANGE);
}
  
// Return current encoder count
long encoder_get_count() {
  return count;
}

float encoder_get_angle() {
  return (2 * PI * count / (2 * PPR)); // Divide by 2 * PPR since we are incrementing on both rising and falling edges
}

float encoder_get_omega() {
  float theta1 = encoder_get_angle();
  delay(VELOCITY_DELAY_MS);
  float theta2 = encoder_get_angle();

  return (1000 * (theta2 - theta1) / VELOCITY_DELAY_MS);
}

#endif