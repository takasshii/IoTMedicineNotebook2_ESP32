#include "queue/queue_utils.h"

QueueUtils *queue;

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
