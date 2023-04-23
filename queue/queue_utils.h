#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t thp[2];
QueueHandle_t xQueue_1;
QueueHandle_t xQueue_2;

class Queue
{
private:
    TaskHandle_t thp[2];
    QueueHandle_t xQueue_1;
    QueueHandle_t xQueue_2;

public:
    void initQueue();
}

#endif /* QUEUE_UTILS_H */