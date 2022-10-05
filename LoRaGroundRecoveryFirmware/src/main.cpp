#include "../include/initlora.h"
#include "../include/defs.h"

TaskHandle_t ListenDownStreamTaskHandle;

void sendLoRaCommand(uint8_t chutePin)
{
  LoRa.beginPacket();
  // send D if drogue , send M if main
  if (chutePin == GROUND_DROGUE_PIN)
  {
    LoRa.print(DROGUE_MESSAGE);
  }
  else if (chutePin == GROUND_MAIN_PIN)
  {
    LoRa.print(MAIN_MESSAGE);
  }
    LoRa.endPacket();
}

void IRAM_ATTR sendEjectDrogueCommand()
{
  sendLoRaCommand(GROUND_DROGUE_PIN);
}
void IRAM_ATTR sendEjectMainCommand()
{
  sendLoRaCommand(GROUND_MAIN_PIN);
}

void ListenDownStreamTask(void *parameter)
{
  for (;;)
  {
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
      while (LoRa.available())
      {
        Serial.print((char)LoRa.read());
      }
    }
    vTaskDelay(5);
  }
}

void setup()
{
  initHeltecLoRa();

  // setup drogue button interrupt
  pinMode(GROUND_DROGUE_PIN, INPUT_PULLUP);                            // initialize pin HIGH
  attachInterrupt(GROUND_DROGUE_PIN, sendEjectDrogueCommand, FALLING); // press drogue push button

  // setup main button interrupt
  pinMode(GROUND_MAIN_PIN, INPUT_PULLUP);                            // initialize pin HIGH
  attachInterrupt(GROUND_MAIN_PIN, sendEjectMainCommand, FALLING); // press drogue push button
  // initialize core 0 tasks
  xTaskCreatePinnedToCore(ListenDownStreamTask, "GetDataTask", 3000, NULL, 1, &ListenDownStreamTaskHandle, pro_cpu);

  // delete setup task
  vTaskDelete(NULL);
}

void loop()
{
}