#ifndef motor_h // include guard
#define motor_h

#include "Stepper.h"
#define STEPS 1 // number of steps to take at a given speed per loop (should be small so you don't block process for long time)

Stepper stepper(0, 0, 0, 0, 0); // dummy constructor

// "Constructor" that calls a real constructor for Stepper
void motor_init(int p1, int p2, int p3, int p4, int steps_per_rev)
{
  stepper = Stepper(steps_per_rev, p1, p2, p3, p4); // real constructor
}

// THIS IS PROBABLY NOT GONNA WORK
void motor_set(int speed)
{
  if (speed > 0)
  {
    stepper.setSpeed(speed);
    stepper.step(STEPS);
  }
  else if (speed < 0)
  {
    stepper.setSpeed(-speed);
    stepper.step(-STEPS);
  }
}

#endif