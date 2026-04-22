#ifndef diagnostic_h
#define diagnostic_h

#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "math.h"

static unsigned long int ticks = 0;
int test_speed = 0;

// We assume the setup() function initializes all the other components
// All we need to do here is start Serial under this assumption
void diag_init()
{
  Serial.begin(9600);
}

// Tests proper function of the encoder
void test_encoder()
{
  Serial.print("Count: ");
  Serial.print(encoder_get_count());
  Serial.print(" Angle: ");
  Serial.print(encoder_get_angle());
  Serial.print(" Angular Velocity: ");
  Serial.println(encoder_get_omega());
}

// Generates oscillating motor speeds (forward and reverse) for complex testing
int motor_function()
{
  return floor((200 * cos((3.14159 / 1000) * (ticks++))));
}

// Tests motor operation in forward direction
// Motor should go in some direction (idk which)
void test_motor_fwd()
{
  motor_set(60);
  Serial.println("Speed: 60");
}

// Tests motor operation in reverse direction
// Motor should go in the opposite direction of test_motor_fwd()
void test_motor_rev()
{
  motor_set(-60);
  Serial.println("Speed: -60");
}

// Complex motor test that continuously changes motor speed and tests both directions
// Motor should move in speed up in one direction, slow down, stop, speed up in the other direction, slow down, stop, etc...
// It should do that until you get bored
void test_motor_cmplx()
{
  test_speed = motor_function();
  motor_set(test_speed);

  Serial.print("Speed: ");
  Serial.println(test_speed);
}

#endif