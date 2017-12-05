#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "stm32f2xx_hal.h"
#include "rtx_os.h"
#include "timer.h"
#include "clock.h"
#include "sms.h"
#include "Serial.h"
#include "JOY.h"

/********************************************/
// The RTOS and HAL need the SysTick for timing. The RTOS wins and gets control
// of SysTick, so we need to route the HAL's tick call to the RTOS's tick.
uint32_t HAL_GetTick(void) {
	return osKernelGetTickCount();
}


// USER button and TAMPER button
// More code than the WAKEUP button handler because
// This can deal with more than one pin
void EXTI15_10_IRQHandler(void){
	// The NVIC automatically clears the flag bit in NVIC for EXTI15_10_IRQHandler by jumping to this vector address
	// However, we must also manually clear the interrupt flag for EXTI15
	// USER Button
	if (EXTI->PR & 1<<15)  // flag bit for EXTI15 is set
	{
		EXTI->PR = 1<<15;   // Acknowledge flag bit by storing a 1 into the flag bit, 0's in other bits are ignored.
		osSemaphoreRelease(semTimer);
	}
	// TAMPER button
	else
	if (EXTI->PR & 1<<13)
	{
		EXTI->PR = 1<<13;
		// Increment minutes
		osSemaphoreRelease(semIncMinutes);
	}
	return;
}

// WAKEUP button
void EXTI0_IRQHandler(void){
	EXTI->PR = 1<<0;
	// Increment hours
	osSemaphoreRelease(semIncHours);
	return;
}

// Serial port
void USART3_IRQHandler(void){
	char character = SER_GetChar();
	if(character == '\r' || character > 'z')character = 0;
	osMessageQueuePut(msgqCharBuffer, &character, NULL, 0);
	return;
}
