#include "queue/queue_utils.h"

Queue *queue;

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
  queue = new Queue();
  queue->initQueue();
}
