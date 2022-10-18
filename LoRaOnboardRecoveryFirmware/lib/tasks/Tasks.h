#ifndef TASKS_H
#define TASKS_H
#include "../definitions/defs.h"

void sendGPSLoRaTask(void *parameter);
void readGPSTask(void *parameter);
void getStatusTask(void *parameter);
void sendStatusLoRaTask(void *parameter);
void OnReceiveTask(void *parameter);
void resumeGPSTasks();

#endif // TASKS_H