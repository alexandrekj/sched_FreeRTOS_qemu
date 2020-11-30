/*==================================================================================================

  sched_FreeRTOS_qemu.c

  Used hardware: STM32F4-Discovery evaluation board (QEMU)

  Example of using the FreeRTOS with the ESFree scheduler on the STM32F4-Discovery board.

  ==================================================================================================
*/

#include "stm32f4_discovery.h"
#include "scheduler.h"
#include "diag/Trace.h"


TaskHandle_t xHandle1 = NULL, xHandle2 = NULL, xHandle3 = NULL;

typedef struct{
	int led;
	int job;
} ledstruct;

ledstruct sled3,sled5,sled6;
int tick = 0;


void vBlink(ledstruct *pvParam){

  int LED = pvParam-> led;
  int job = pvParam-> job;

  trace_printf("%s, job: %d, tick: %d \n", pcTaskGetName(NULL), pvParam->job++,tick++);
  BSP_LED_Toggle((uint32_t)pvParam);
} // vBlink


void main(void){
  __HAL_RCC_GPIOD_CLK_ENABLE();
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  sled3.led = LED3;
  sled3.job = 1;
  sled5.led = LED5;
  sled5.job = 1;
  sled6.led = LED6;
  sled6.job = 1;


  vSchedulerInit();

  vSchedulerPeriodicTaskCreate(vBlink, "Task1", configMINIMAL_STACK_SIZE, &sled3, 1, &xHandle1, pdMS_TO_TICKS(000), pdMS_TO_TICKS(250), pdMS_TO_TICKS(001), pdMS_TO_TICKS(250));
  vSchedulerPeriodicTaskCreate(vBlink, "Task2", configMINIMAL_STACK_SIZE, &sled5, 1, &xHandle2, pdMS_TO_TICKS(250), pdMS_TO_TICKS(250), pdMS_TO_TICKS(001), pdMS_TO_TICKS(250));
  vSchedulerPeriodicTaskCreate(vBlink, "Task3", configMINIMAL_STACK_SIZE, &sled6, 1, &xHandle3, pdMS_TO_TICKS(125), pdMS_TO_TICKS(500), pdMS_TO_TICKS(001), pdMS_TO_TICKS(500));

  vSchedulerStart();
  while(1);
} // main
