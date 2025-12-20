#include <Arduino.h>
#include "pins.h"
#include "motor_bts7960.h"

// Initialize motor driver pins
void motorInit() {
  pinMode(RPWM, OUTPUT);  // Forward PWM as output
  pinMode(LPWM, OUTPUT);  // Backward PWM as output
  pinMode(REN, OUTPUT);   // Right enable output
  pinMode(LEN, OUTPUT);   // Left enable output

  digitalWrite(REN, HIGH); // Enable right side
  digitalWrite(LEN, HIGH); // Enable left side

  motorSet(0); // Set motor to stop initially
}

// Set motor speed
void motorSet(int pwm) {
  pwm = constrain(pwm, -255, 255); // Limit PWM between -255 and 255

  if (pwm > 0) {
    analogWrite(RPWM, pwm); // Forward rotation
    analogWrite(LPWM, 0);   // Stop backward
  } else if (pwm < 0) {
    analogWrite(RPWM, 0);   // Stop forward
    analogWrite(LPWM, -pwm); // Backward rotation
  } else {
    analogWrite(RPWM, 0);   // Stop both
    analogWrite(LPWM, 0);
  }
}
