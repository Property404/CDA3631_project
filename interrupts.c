#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "stm32f2xx_hal.h"
#include "rtx_os.h"
#include "timer.h"
#include "clock.h"
#include "sms.h"
#include "Serial.h"

// USER button and TAMPER button
// More code than the WAKEUP button handler because
// This can deal with more than one pin
void EXTI15_10_IRQHandler(void){
	static int32_t i = 0;  // Reminder that an ISR can create local memory with a "static" modifier
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

	else {// EXTI interrupts for pins 10-14 can be sequentially checked for here if they are enabled/configured
		// otherwise this NVIC interrupt should have been triggered, so
		while(1) {i++;} // scream and die	
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
	osSemaphoreRelease(semTimer);
	if(character == '\r')character = 0;
	if(osMessageQueuePut(msgqCharBuffer, &character, NULL, 0) != osOK){
		while(1){}
	}
	return;
}
