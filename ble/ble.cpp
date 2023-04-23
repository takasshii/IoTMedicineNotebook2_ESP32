#include "ble.h"

#define SERVICE_UUID "0000180f-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "00002a19-0000-1000-8000-00805f9b34fb"
#define DEVICE_NAME "ESP_32"

const int RED_LED = 23;

static BLE *instance = NULL;

BLE::BLE() {}

BLE::BLE(FuncPtrInt f)
{
    instance = this;
    callBackEvent = f;
}

// 接続されたときに呼び出されるコールバック
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        instance->callBackEvent(CONNECTED_BLE_SERVER);
        Serial.println("Connected");
    }

    void onDisconnect(BLEServer *pServer)
    {
        instance->callBackEvent(DISCONNECTED_BLE_SERVER);
        digitalWrite(RED_LED, LOW);
        Serial.println("Disconnected");
    }
};

// ペリフェラルからのリクエストを処理するコールバック
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();
        Serial.println(value.c_str());

        // レスポンスを返すために、setValue()メソッドを使用する
        pCharacteristic->setValue("Received message: " + value);
        pCharacteristic->notify();

        if (strcmp(value.c_str(), "C0A0") == 0)
        {
            instance->callBackEvent(INITIAL_COMPLETED_WEIGHT);
            digitalWrite(RED_LED, HIGH);
            return;
        }
        if (strcmp(value.c_str(), "C0A1") == 0)
        {
            instance->callBackEvent(READ_SERVO_COMMAND);
            return;
        }
    }
};

void BLE::initBLE()
{
    instance->callBackEvent(INITIAL_BLE_SERVER);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);

    BLEDevice::init(DEVICE_NAME);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    instance->callBackEvent(COMPLETE_BLE_SERVER);
}

void BLE::writeCharacteristic(String value)
{
    // Convert String to char array
    char charValue[value.length() + 1];
    value.toCharArray(charValue, sizeof(charValue));

    // Set BLE Characteristic value
    pCharacteristic->setValue(charValue);
    pCharacteristic->notify();
}