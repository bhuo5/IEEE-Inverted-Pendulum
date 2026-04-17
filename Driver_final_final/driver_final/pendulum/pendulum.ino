#include <Arduino.h>
#include "encoder.h"
#include "motor.h"
#include "diagnostic.h"
#include "pid.h"

#define KP -15000 // proportion coefficient
#define KI 0 // integral coefficient
#define KD 0 // derivative coefficient
#define SPD_MAX 300 // max forward rpm (PID limits)
#define SPD_MIN -300  // max reverse rpm (PID limits)
#define TARGET 0.00  // target angle is 0 degrees from where we let it go
#define DELAY_MS 10  // 10 ms delay in main loop

// ONLY CHANGE TO ZER0 (0) IF DIAGNOSTICS PASS AND PHYSICAL SETUP IS FULLY ASSEMBLED
// If diagnostics don't pass or assembly is incomplete then the code will either straight up not work (best case)
// Or some physical/electrical components are gonna get nuked (worst case)
#define DIAGNOSTIC_MODE 0
#define JOYSTICK_MODE 0

const int steps_per_rev = 200;

float actual_angle = 0.0f;
float omega = 0.0f;
float set_speed = 0.0f;
unsigned long int time_i = 0;
unsigned long int time_f = 0;
bool first_loop = true;

void setup() {
  motor_init(2, 3, 4, 5, steps_per_rev);
  encoder_init(16, 18);
  diag_init();
  pid_init(KP, KI, KD, SPD_MIN, SPD_MAX);
  pid_reset_integrator();

  pinMode(A0, INPUT);
}

#if DIAGNOSTIC_MODE == 0
float prev_angle = encoder_get_angle();
unsigned long prev_time_us = 0;

void loop() {
  unsigned long now_us = micros();
  float dt = (prev_time_us == 0) ? 0.001f : (now_us - prev_time_us) / 1000000.0f;
  prev_time_us = now_us;

  actual_angle = encoder_get_angle();

  omega = (actual_angle - prev_angle) / dt;
  prev_angle = actual_angle;

  set_speed = pid_compute(TARGET, actual_angle, omega, dt);
  motor_set(set_speed);
}
#endif

#if DIAGNOSTIC_MODE == 1 && JOYSTICK_MODE == 0
void loop()
{
  test_encoder();
}
#endif

#if DIAGNOSTIC_MODE == 1 && JOYSTICK_MODE == 1
void loop() {
    if (analogRead(A0) >= 820)
    {
      motor_set(290 * (analogRead(A0) - 820) / 203);
      Serial.print("Speed: ");
      Serial.print(290 * (analogRead(A0) - 820) / 203);
      Serial.print(" Read: ");
      Serial.println(analogRead(A0));
    }
    else
    {
      motor_set(290 * (analogRead(A0) - 820) / 820);
      Serial.print("Speed: ");
      Serial.print(290 * (analogRead(A0) - 820) / 820);
      Serial.print(" Read: ");
      Serial.println(analogRead(A0));
    }

}
#endif