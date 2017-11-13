/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "Board_GLCD.h"
#include "Board_LED.h"
#include "Board_Buttons.h"
#include "GLCD_Config.h"
#include "stm32f2xx_hal.h"
#include "rtx_os.h"
#include "rtosClockObjects.h"    // specific to this project
extern GLCD_FONT GLCD_Font_16x24;

// Note that the main file declares the space for all the system's variables (someone has to),
// and "extern" declarations are in the rtosClockObjects.h file so other files can find them
uint32_t hour=23, minute=59, second=57;
uint32_t timer_minute = 0, timer_second = 0, timer_millisecond = 0;
osMutexId_t mutHour, mutMinute, mutSecond, mutTimerMinute, mutTimerSecond, mutTimerMillisecond;
osSemaphoreId_t semIncMinutes;
osSemaphoreId_t semIncHours;
osSemaphoreId_t semTimer, semIncTimerSeconds;

/********************************************/
// The RTOS and HAL need the SysTick for timing. The RTOS wins and gets control
// of SysTick, so we need to route the HAL's tick call to the RTOS's tick.
uint32_t HAL_GetTick(void) { 
  return osKernelGetTickCount();
}


/*----------------------------------------------------------------------------
 * HW Init - since the HAL depends on a periodic timer, we need the RTOS
 * in order for several HW devices to initialize correctly, like the GLCD
 *---------------------------------------------------------------------------*/
void app_hw_init (void *argument) {
	
	GLCD_Initialize();
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_PURPLE);
	GLCD_ClearScreen(); 
	GLCD_SetFont(&GLCD_Font_16x24);
	
	// Create other threads here so that all initialization is done before others get scheduled.
	Init_thdIncHours();
	Init_thdDisplayTimer();
	Init_thdDisplayClock();
	Init_thdIncSeconds();
	Init_thdIncMinutes();
	Init_thdIncTimer();
	Init_thdIncTimerSeconds();
	
	
	
	
	osThreadExit(); // job is done, thread suicide. There better be other threads created...
}

static osMutexId_t newMutexOrDie(){
	osMutexId_t mut_id = osMutexNew(NULL);
	if(mut_id == NULL){
		while(1){}
	}
	return mut_id;
}

int main (void) {
 
	SystemCoreClockUpdate();	// always first, make sure the clock freq. is current
	osKernelInitialize();     // Initialize CMSIS-RTOS
	HAL_Init();   			
	Buttons_Initialize();			// Does NOT use a HAL tick during initialization
	LED_Initialize();

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

	

	// Minutes and hours have a maximum of two
	// Because thdIncMinutes and thdIncHours are unblocked
	// In thdIncSeconds/thdIncMinutes as well as IQRs
	semIncMinutes = osSemaphoreNew(2, 0, NULL);
	semIncHours = osSemaphoreNew(2, 0, NULL);
	semTimer = osSemaphoreNew(1, 0, NULL);
	semIncTimerSeconds = osSemaphoreNew(1, 0, NULL);

	
	// Check for problems with semaphore/ef creation
	if(semIncMinutes == NULL)
		while(1){}
	if(semIncHours == NULL)
		while(1){}
	if(semTimer == NULL)
		while(1){}
			

	mutHour = newMutexOrDie();
	mutMinute = newMutexOrDie();
	mutSecond = newMutexOrDie();
	mutTimerMillisecond = newMutexOrDie();
	mutTimerMinute = newMutexOrDie();
	mutTimerSecond = newMutexOrDie();
	
	
	osThreadNew(app_hw_init, NULL, NULL); // Create application main thread
	osKernelStart();                      // Start thread execution
	for (;;) {}
}



// USER button and TAMPER button
// More code than the WAKEUP button handler because
// This can deal with more than one pin
void EXTI15_10_IRQHandler(void)
{
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

