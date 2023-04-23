#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/servo/servo.cpp"
#include "servo.h"

const int SERVO_PIN_1 = 15;
const int SERVO_PIN_2 = 16;

static Servo *instance = NULL;

Servo::Servo() {}

Servo::Servo(FuncPtrInt f)
{
  instance = this;
  callBackEvent = f;
}

double Servo::movingServo()
{
  instance->callBackEvent(MOVING_SERVO);
  servo1.write(0);
  servo2.write(0);
  delay(1000);

  servo1.write(180);
  servo2.write(180);
  delay(1000);

  servo1.write(0);
  servo2.write(0);
  delay(1000);
  instance->callBackEvent(COMPLETED_SERVO)
}

void Servo::initServo()
{
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
}
