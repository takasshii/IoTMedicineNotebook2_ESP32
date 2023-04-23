# 1 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino"
# 2 "/Users/takashi/Documents/Arduino/IoT_medicine_notebook_2/IoT_medicine_notebook_2.ino" 2

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
