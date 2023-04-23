#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/queue_utils.cpp"
#include "queue_utils.h"

#include "servo_utils.h"
#include "ble.h"
#include "weight.h"

static QueueUtils *instance = NULL;

TaskHandle_t thp[2];
QueueHandle_t xQueue_1;
QueueHandle_t xQueue_2;

Weight *weight;
BLE *ble;
ServoUtils *servo;

QueueUtils::QueueUtils()
{
    instance = this;
}

void QueueUtils::initQueue()
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
        break;
    case MEASURING_WEIGHT:
        break;
    case COMPLETED_WEIGHT:
        break;
    default:
        break;
    }
}

void sendWeight(float *result)
{
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

void initBle()
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
        break;
    case COMPLETE_BLE_SERVER:
        break;
    case CONNECTED_BLE_SERVER:
        break;
    case DISCONNECTED_BLE_SERVER:
        break;
    case INITIAL_READ_CHARACTERISTIC:
        // LEDを点灯させる

        break;
    case READ_SERVO_COMMAND:
        // Servoを動かす
        servo->movingServo();
        break;
    case SEND_COMPLETED_COMMAND:
        break;
    default:
        break;
    }
}

void writeCharacteristic()
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
    servo = new Servo(ptr);
    servo->initServo();
}

// 　WeightEventのCallback
void servoEvent(SERVO_STATE event)
{
    switch (event)
    {
    case INITIAL_COMPLETED_SERVO:
        break;
    case MOVING_SERVO:
        break;
    case COMPLETED_SERVO:
        // 完了したことをBLEサーバーで送信するために1を送信する
        xQueueSend(xQueue_2, 1, 0);
        break;
    default:
        break;
    }
}
