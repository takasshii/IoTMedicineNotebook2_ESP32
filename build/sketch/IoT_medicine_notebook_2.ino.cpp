#include <Arduino.h>
#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
#include "servo_utils.h"
#include "ble.h"
#include "weight.h"

TaskHandle_t thp[2];
QueueHandle_t xQueue_1;
QueueHandle_t xQueue_2;

const int RED_LED = 5;

Weight *weight;
BLE *ble;
ServoUtils *servo;

#line 15 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void setup();
#line 22 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void loop();
#line 26 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void initQueue();
#line 41 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void initWeight();
#line 49 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void weightEvent(WEIGHT_STATE event);
#line 67 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void sendWeight(void *args);
#line 81 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void initBLE();
#line 89 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void bleEvent(BLE_STATE event);
#line 124 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void writeCharacteristic(void *args);
#line 147 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void initServo();
#line 155 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void servoEvent(SERVO_STATE event);
#line 15 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  initQueue();
}

void loop()
{
}

void initQueue()
{
  initWeight();
  initBLE();
  initServo();

  xQueue_1 = xQueueCreate(10, 16);
  xQueue_2 = xQueueCreate(10, 32);

  // スレッドの準備
  xTaskCreatePinnedToCore(sendWeight, "sendWeight", 8192, NULL, 1, &thp[0], 1);
  xTaskCreatePinnedToCore(writeCharacteristic, "writeCharacteristic", 8192, NULL, 2, &thp[1], 0);
}

// Weightの初期化
void initWeight()
{
  void (*ptr)(WEIGHT_STATE) = &weightEvent;
  weight = new Weight(ptr);
  weight->initWeight();
}

// 　WeightEventのCallback
void weightEvent(WEIGHT_STATE event)
{
  switch (event)
  {
  case INITIAL_COMPLETED_WEIGHT:
    Serial.println("INITIAL_COMPLETED_WEIGHT");
    break;
  case MEASURING_WEIGHT:
    Serial.println("MEASURING_WEIGHT");
    break;
  case COMPLETED_WEIGHT:
    Serial.println("COMPLETED_WEIGHT");
    break;
  default:
    break;
  }
}

void sendWeight(void *args)
{
  double result;
  while (1)
  {
    result = weight->measureWeight();
    if (result != NULL)
    {
      xQueueSend(xQueue_1, &result, 0);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void initBLE()
{
  void (*ptr)(BLE_STATE) = &bleEvent;
  ble = new BLE(ptr);
  ble->initBLE();
}

// 　WeightEventのCallback
void bleEvent(BLE_STATE event)
{
  switch (event)
  {
  case INITIAL_BLE_SERVER:
    Serial.println("INITIAL_BLE_SERVER");
    break;
  case COMPLETE_BLE_SERVER:
    Serial.println("COMPLETE_BLE_SERVER");
    break;
  case CONNECTED_BLE_SERVER:
    Serial.println("CONNECTED_BLE_SERVER");
    break;
  case DISCONNECTED_BLE_SERVER:
    Serial.println("DISCONNECTED_BLE_SERVER");
    digitalWrite(RED_LED, LOW);
    break;
  case INITIAL_READ_CHARACTERISTIC:
    // LEDを点灯させる
    Serial.println("INITIAL_READ_CHARACTERISTIC");
    digitalWrite(RED_LED, HIGH);
    break;
  case READ_SERVO_COMMAND:
    Serial.println("READ_SERVO_COMMAND");
    // Servoを動かす
    servo->movingServo();
    break;
  case SEND_COMPLETED_COMMAND:
    Serial.println("SEND_COMPLETED_COMMAND");
    break;
  default:
    break;
  }
}

void writeCharacteristic(void *args)
{
  double weightResult;
  int servoCompleted;

  while (1)
  {
    xQueueReceive(xQueue_1, &weightResult, portMAX_DELAY);
    xQueueReceive(xQueue_2, &servoCompleted, portMAX_DELAY);
    if (weightResult != NULL)
    {
      ble->writeCharacteristic("Change Weight");
      delay(1);
    }
    if (servoCompleted != NULL)
    {
      ble->writeCharacteristic("Moved Servo");
      delay(1);
    }
  }
  delay(1);
}

void initServo()
{
  void (*ptr)(SERVO_STATE) = &servoEvent;
  servo = new ServoUtils(ptr);
  servo->initServo();
}

// 　WeightEventのCallback
void servoEvent(SERVO_STATE event)
{
  int sendCompleted = 1;
  switch (event)
  {
  case INITIAL_COMPLETED_SERVO:
    Serial.println("INITIAL_COMPLETED_SERVO");
    break;
  case MOVING_SERVO:
    Serial.println("MOVING_SERVO");
    break;
  case COMPLETED_SERVO:
    Serial.println("COMPLETED_SERVO");
    // 完了したことをBLEサーバーで送信するために1を送信する
    xQueueSend(xQueue_2, &sendCompleted, 0);
    break;
  default:
    break;
  }
}
