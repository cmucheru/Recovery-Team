#include "../include/initlora.h"
#include "../include/defs.h"

TaskHandle_t ListenDownStreamTaskHandle = NULL;
TaskHandle_t DrogueInterruptTaskHandle = NULL;
TaskHandle_t MainChuteInterruptTaskHandle = NULL;

void sendLoRaCommand(uint8_t chutePin)
{
  LoRa.beginPacket();
  // send D if drogue , send M if main
  if (chutePin == GROUND_DROGUE_PIN)
  {
    LoRa.print(DROGUE_MESSAGE);
    if (LoRa.endPacket())
    {
      debugf("Sent %s\n", DROGUE_MESSAGE);
    }
  }
  else if (chutePin == GROUND_MAIN_PIN)
  {
    LoRa.print(MAIN_MESSAGE);

    if (LoRa.endPacket())
    {
      debugf("Sent %s\n", MAIN_MESSAGE);
    }
  }
}

void IRAM_ATTR sendEjectDrogueCommand()
{
  BaseType_t checkIfYieldRequired;
  checkIfYieldRequired = xTaskResumeFromISR(DrogueInterruptTaskHandle);
  portYIELD_FROM_ISR(checkIfYieldRequired);
}
void IRAM_ATTR sendEjectMainCommand()
{
  BaseType_t checkIfYieldRequired;
  checkIfYieldRequired = xTaskResumeFromISR(MainChuteInterruptTaskHandle);
  portYIELD_FROM_ISR(checkIfYieldRequired);
}

void DrogueInterruptTask(void *parameter)
{
  for (;;)
  {
    vTaskSuspend(NULL);
    sendLoRaCommand(GROUND_DROGUE_PIN);
  }
}
void MainChuteInterruptTask(void *parameter)
{
  for (;;)
  {
    vTaskSuspend(NULL);
    sendLoRaCommand(GROUND_MAIN_PIN);
  }
}

void ListenDownStreamTask(void *parameter)
{
  for (;;)
  {
    int packetSize = LoRa.parsePacket();
    char gpsString[60];
    for (int i = 0; i < packetSize; i++)
    {
      gpsString[i] = (char)LoRa.read();
    }
    debugln(gpsString);
    vTaskDelay(5);
  }
}
void setPinModes()
{
  // setup drogue button interrupt
  pinMode(GROUND_DROGUE_PIN, INPUT_PULLUP);                            // initialize pin HIGH
  attachInterrupt(GROUND_DROGUE_PIN, sendEjectDrogueCommand, FALLING); // press drogue push button

  // setup main button interrupt
  pinMode(GROUND_MAIN_PIN, INPUT_PULLUP);                          // initialize pin HIGH
  attachInterrupt(GROUND_MAIN_PIN, sendEjectMainCommand, FALLING); // press drogue push button
}

void setup()
{
  initHeltecLoRa();

  // initialize core 0 tasks
  xTaskCreatePinnedToCore(ListenDownStreamTask, "ListenDownStreamTask", 3000, NULL, 1, &ListenDownStreamTaskHandle, pro_cpu);

  // initialize core 1 tasks
  xTaskCreatePinnedToCore(DrogueInterruptTask, "DrogueInterruptTask", 3000, NULL, 1, &DrogueInterruptTaskHandle, app_cpu);
  xTaskCreatePinnedToCore(MainChuteInterruptTask, "MainChuteInterruptTask", 3000, NULL, 1, &MainChuteInterruptTaskHandle, app_cpu);

  // delete setup task
  vTaskDelete(NULL);
}

void loop()
{
}