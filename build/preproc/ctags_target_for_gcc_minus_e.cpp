# 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
# 2 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 2
# 3 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 2
# 4 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 2

TaskHandle_t thp[2];
QueueHandle_t xQueue_1;
QueueHandle_t xQueue_2;

const int RED_LED = 5;

Weight *weight;
BLE *ble;
ServoUtils *servo;

void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, 0x02);
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

  xQueue_1 = xQueueGenericCreate( ( 10 ), ( 16 ), ( ( ( uint8_t ) 0U ) ) );
  xQueue_2 = xQueueGenericCreate( ( 10 ), ( 32 ), ( ( ( uint8_t ) 0U ) ) );

  // スレッドの準備
  xTaskCreatePinnedToCore(sendWeight, "sendWeight", 8192, 
# 36 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 3 4
                                                         __null
# 36 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
                                                             , 1, &thp[0], 1);
  xTaskCreatePinnedToCore(writeCharacteristic, "writeCharacteristic", 8192, 
# 37 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 3 4
                                                                           __null
# 37 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
                                                                               , 2, &thp[1], 0);
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
    if (result != 
# 73 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 3 4
                 __null
# 73 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
                     )
    {
      xQueueGenericSend( ( xQueue_1 ), ( &result ), ( 0 ), ( ( BaseType_t ) 0 ) );
    }
    vTaskDelay(1000 / ( ( TickType_t ) 1000 / ( 1000 ) ));
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
    digitalWrite(RED_LED, 0x0);
    break;
  case INITIAL_READ_CHARACTERISTIC:
    // LEDを点灯させる
    Serial.println("INITIAL_READ_CHARACTERISTIC");
    digitalWrite(RED_LED, 0x1);
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
    xQueueGenericReceive( ( xQueue_1 ), ( &weightResult ), ( ( TickType_t ) 0xffffffffUL ), ( ( BaseType_t ) 0 ) );
    xQueueGenericReceive( ( xQueue_2 ), ( &servoCompleted ), ( ( TickType_t ) 0xffffffffUL ), ( ( BaseType_t ) 0 ) );
    if (weightResult != 
# 133 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 3 4
                       __null
# 133 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
                           )
    {
      ble->writeCharacteristic("Change Weight");
      delay(1);
    }
    if (servoCompleted != 
# 138 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 3 4
                         __null
# 138 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
                             )
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
    xQueueGenericSend( ( xQueue_2 ), ( &sendCompleted ), ( 0 ), ( ( BaseType_t ) 0 ) );
    break;
  default:
    break;
  }
}
