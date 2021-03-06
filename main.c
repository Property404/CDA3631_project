/*----------------------------------------------------------------------------
 * Dagan Martinez
 * CDA3631, Fall 2017, Florida Polytechnic University
 * Professor: Dr. David Foster
 * https://github.com/Property404/CDA3631_project
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "Board_Buttons.h"
#include "stm32f2xx_hal.h"
#include "rtx_os.h"
#include "timer.h"
#include "clock.h"
#include "Serial.h"
#include "JOY.h"
#include "sms.h"
#include "display.h"

/*----------------------------------------------------------------------------
 * HW Init - since the HAL depends on a periodic timer, we need the RTOS
 * in order for several HW devices to initialize correctly, like the GLCD
 *---------------------------------------------------------------------------*/
void app_hw_init (void *argument) {
	// Initialize object groups
	initializeClockObjects();
	initializeTimerObjects();
	initializeSMSObjects();
	
	// GLCD etc
	setupDisplay();

	// Create other threads here so that all initialization is done before others get scheduled.
	initializeSMSThreads();
	initializeTimerThreads();
	initializeClockThreads();

	osThreadExit(); // job is done, thread suicide. There better be other threads created...
}

int main (void) {
 
	SystemCoreClockUpdate();	// always first, make sure the clock freq. is current
	osKernelInitialize();     // Initialize CMSIS-RTOS
	HAL_Init();   			
	Buttons_Initialize();			// Does NOT use a HAL tick during initialization

	// Setup serial connection
	SER_Init(115200);
	NVIC_SetPriority(USART3_IRQn, 5);
	NVIC_EnableIRQ(USART3_IRQn);
	USART3 -> CR1 |= USART_CR1_RXNEIE;
	
	// And joystick
	JOY_Init();
	
	// To configure the USER and WAKEUP buttons to generate an interrupt...
	// Enable EXTI15, EXTI13 and EXTI0 - Edge interrupt for all pin 15's, 13's 0's
	EXTI->IMR |= (1<<15) | (1<<13) | (1<<0);  
	// configure for rising edge	
	EXTI->FTSR |= (1<<15) | (1<<13) | (1<<0);  		
	RCC->APB2ENR |= 1<<14; 		// Turn on bus clock for the system configuration to turn on edge detector
	// Set the EXTI control register to look at PG15, PC13, and PA0
	SYSCFG->EXTICR[3] =	(SYSCFG->EXTICR[3] & ~0x0000F000)
		| SYSCFG_EXTICR4_EXTI15_PG
		| SYSCFG_EXTICR4_EXTI13_PC
		| SYSCFG_EXTICR1_EXTI0_PA;
	//NOTE: these will have separate interrupts

													
	/* The EXTI, or external interrupt module, is now sending flags to the NVIC for the interrupts
	coming from pins 10-15 (it routes all 6 to one IRQ to save space). Next, we have to enable the
	NVIC to pay attention to this interrupt.   */

	NVIC->ISER[EXTI15_10_IRQn/32] |= 1<< (EXTI15_10_IRQn%32);
	NVIC->ISER[EXTI0_IRQn/32] |= 1<<(EXTI0_IRQn%32);
			

	osThreadNew(app_hw_init, NULL, NULL); // Create application main thread
	osKernelStart();                      // Start thread execution
	for (;;) {}
}
