#ifndef BLE_H_
#define BLE_H_

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

typedef enum
{
    INITIAL_BLE_SERVER,
    COMPLETE_BLE_SERVER,
    CONNECTED_BLE_SERVER,
    DISCONNECTED_BLE_SERVER,
    INITIAL_READ_CHARACTERISTIC,
    READ_SERVO_COMMAND,
    SEND_COMPLETED_COMMAND,
} BLE_STATE;

class BLE
{
private:
    BLEServer *pServer;
    BLECharacteristic *pCharacteristic;
    typedef void (*FuncPtrInt)(BLE_STATE);

public:
    FuncPtrInt callBackEvent;
    BLE();
    BLE(FuncPtrInt f);
    void initBLE();
    void writeCharacteristic(String value);
};

#endif