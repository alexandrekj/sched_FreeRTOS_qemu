/*==================================================================================================
  sched_FreeRTOS_qemu.c
  Used hardware: STM32F4-Discovery evaluation board (QEMU)
  Example of using the FreeRTOS with the ESFree scheduler on the STM32F4-Discovery board.
  ==================================================================================================
*/

// testar
// condições de bloqueio - emitir prints

#include "stm32f4_discovery.h"
#include "scheduler.h"
#include "diag/Trace.h"
#include "semphr.h"
#include "task.h"

TaskHandle_t xHandle1 = NULL, xHandle2 = NULL, xHandle3 = NULL, xHandle4 = NULL;
SemaphoreHandle_t xMutex1 = NULL, xMutex2 = NULL;

void vBlink1(void *pvParam){

  pdMS_TO_TICKS(001);
  if(xSemaphoreTake(xMutex1, 1) == pdTRUE){
      trace_puts("Task1 - Mutex1 - Entrando na secao critica, tick: %d", xTaskGetTickCount());
      pdMS_TO_TICKS(001);
   	  trace_puts("Task1 - Mutex1 - Saindo da secao critica, tick: %d", xTaskGetTickCount());
  	  xSemaphoreGive(xMutex1);
  } // if

  if(xSemaphoreTake(xMutex2, 1) == pdTRUE){
      trace_puts("Task1 - Mutex2 - Entrando na secao critica, tick: %d", xTaskGetTickCount());
      pdMS_TO_TICKS(001);
      trace_puts("Task1 - Mutex2 - Saindo da secao critica, tick: %d", xTaskGetTickCount());
      xSemaphoreGive(xMutex2);
  } // if
  pdMS_TO_TICKS(001);
} // vBlink1

void vBlink2(void *pvParam){
	pdMS_TO_TICKS(001);
  if(xSemaphoreTake(xMutex2, 1) == pdTRUE){
        trace_puts("Task2 - Mutex2 - Entrando na secao critica, tick: %d", xTaskGetTickCount());
        pdMS_TO_TICKS(002);
        trace_puts("Task2 - Mutex2 - Saindo da secao critica, tick: %d", xTaskGetTickCount());
        xSemaphoreGive(xMutex2);
    } // if
  pdMS_TO_TICKS(001);
} // vBlink2

void vBlink3(void *pvParam){
  trace_puts("Task3, tick: %d", xTaskGetTickCount());
  pdMS_TO_TICKS(001);
} // vBlink3

void vBlink4(void *pvParam){
	pdMS_TO_TICKS(001);
    if(xSemaphoreTake(xMutex1, 1) == pdTRUE){
	    trace_puts("Task1 - Mutex1 - Entrando na secao critica, tick: %d",xTaskGetTickCount());
	    pdMS_TO_TICKS(004);
	    trace_puts("Task1 - Mutex1 - Saindo da secao critica, tick: %d", xTaskGetTickCount());
	  	xSemaphoreGive(xMutex1);
	  } // if
    pdMS_TO_TICKS(001);
} // vBlink4


void main(void){
  __HAL_RCC_GPIOD_CLK_ENABLE();

  char c1 = 'a';
  char c2 = 'b';
  char c3 = 'c';
  char c4 = 'e';

  vSchedulerInit();
  // delay, T(periodo), E(tempo de execução), D (prazo)
  vSchedulerPeriodicTaskCreate(vBlink1, "Task1", configMINIMAL_STACK_SIZE, &c1, 4, &xHandle1, pdMS_TO_TICKS(016), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(004), pdMS_TO_TICKS(1000));
  vSchedulerPeriodicTaskCreate(vBlink2, "Task2", configMINIMAL_STACK_SIZE, &c2, 3, &xHandle2, pdMS_TO_TICKS(014), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(004), pdMS_TO_TICKS(1000));
  vSchedulerPeriodicTaskCreate(vBlink3, "Task3", configMINIMAL_STACK_SIZE, &c3, 2, &xHandle3, pdMS_TO_TICKS(014), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(001), pdMS_TO_TICKS(1000));
  vSchedulerPeriodicTaskCreate(vBlink4, "Task4", configMINIMAL_STACK_SIZE, &c4, 1, &xHandle4, pdMS_TO_TICKS(012), pdMS_TO_TICKS(1000), pdMS_TO_TICKS(006), pdMS_TO_TICKS(1000));
  xMutex1 = xSemaphoreCreateMutex();
  xMutex2 = xSemaphoreCreateMutex();


  vSchedulerStart();
  while(1);
} // main
