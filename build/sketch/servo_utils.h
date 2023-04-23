#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/servo_utils.h"
#ifndef SERVO_UTILS_H_
#define SERVO_UTILS_H_

#include <Servo.h>

typedef enum
{
    INITIAL_COMPLETED_SERVO,
    MOVING_SERVO,
    COMPLETED_SERVO,
} SERVO_STATE;

class ServoUtils
{
private:
    Servo servo1;
    Servo servo2;
    typedef void (*FuncPtrInt)(SERVO_STATE);

public:
    FuncPtrInt callBackEvent;
    ServoUtils();
    ServoUtils(FuncPtrInt f);
    void initServo();
    void movingServo();
};

#endif