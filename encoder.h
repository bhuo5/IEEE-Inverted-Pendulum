// as said in the file "encoder.py" and group chat microPython may not be fast enough for a PID controller since
// we need the feedback as fast as possible. I wrote the code in Arduino/C++
#ifndef ENCODER_H
#define ENCODER_H

// Initialize encoder pins and interrupts
void encoderInit();

// Get current encoder count
long encoderGetCount();

#endif
