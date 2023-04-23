#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/servo/servo.h"
#ifndef SERVO_H_
#define SERVO_H_

#include <Servo.h>

typedef enum
{
    INITIAL_COMPLETED_SERVO,
    MOVING_SERVO,
    COMPLETED_SERVO,
} SERVO_STATE;

class Servo
{
private:
    Servo servo1;
    Servo servo2;
    typedef void (*FuncPtrInt)(SERVO_STATE);

public:
    FuncPtrInt callBackEvent;
    Servo();
    Servo(FuncPtrInt f);
    void initServo();
    double movingServo();
};

#endif