#include <Arduino.h>
#include "encoder.h"
#include "motor.h"
#include "diagnostic.h"
#include "pid.h"

#define KP 10 // proportion coefficient
#define KI 10 // integral coefficient
#define KD 10 // derivative coefficient
#define SPD_MAX 200 // max forward rpm (PID limits)
#define SPD_MIN -200  // max reverse rpm (PID limits)
#define TARGET 0.00  // target angle is 0 degrees from where we let it go
#define DELAY_MS 10  // 10 ms delay in main loop

// ONLY CHANGE TO ZER0 (0) IF DIAGNOSTICS PASS AND PHYSICAL SETUP IS FULLY ASSEMBLED
// If diagnostics don't pass or assembly is incomplete then the code will either straight up not work (best case)
// Or some physical/electrical components are gonna get nuked (worst case)
#define DIAGNOSTIC_MODE 1
#define JOYSTICK_MODE 1

const int steps_per_rev = 200;

float actual_angle = 0.0f;
float omega = 0.0f;
float set_speed = 0.0f;
unsigned long int time_i = 0;
unsigned long int time_f = 0;
bool first_loop = true;

void setup() {
  motor_init(2, 3, 4, 5, steps_per_rev);
  encoder_init(0, 1);
  diag_init();
  pid_init(KP, KI, KD, SPD_MIN, SPD_MAX);
  pid_reset_integrator();

  pinMode(A0, INPUT);
}

#if DIAGNOSTIC_MODE == 0
void loop() {
  actual_angle = encoder_get_angle();
  omega = encoder_get_omega();
  time_f = millis();
  set_speed = pid_compute(TARGET, actual_angle, omega, first_loop ? 0.003 : (float) (time_f - time_i) / 1000); // put a convincing number of 3 ms first loop dt
  time_i = millis();
  motor_set(set_speed);

  first_loop = false;
}
#endif

#if DIAGNOSTIC_MODE == 1 && JOYSTICK_MODE == 0
void loop()
{
  test_motor_cmplx();
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