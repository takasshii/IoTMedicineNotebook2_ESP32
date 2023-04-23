#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/servo_utils.cpp"
#include "servo_utils.h"

const int SERVO_PIN_1 = 15;
const int SERVO_PIN_2 = 16;

static ServoUtils *instance = NULL;

ServoUtils::ServoUtils() {}

ServoUtils::ServoUtils(FuncPtrInt f)
{
  instance = this;
  callBackEvent = f;
}

void ServoUtils::movingServo()
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
  instance->callBackEvent(COMPLETED_SERVO);
}

void ServoUtils::initServo()
{
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
}
