#include <Arduino.h>
#line 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
#include "queue_utils.h"

QueueUtils *queue;

#line 5 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void setup();
#line 11 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void loop();
#line 15 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void initQueue();
#line 5 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
void setup()
{
  Serial.begin(115200);
  initQueue();
}

void loop()
{
}

void initQueue()
{
  queue = new QueueUtils();
  queue->initQueue();
}

