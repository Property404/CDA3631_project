#ifndef COMMON_HEADER_CLOCK
#define COMMON_HEADER_CLOCK
#include "time_components.h"
// shared variables and mutexes
extern uint32_t hour; 
extern uint32_t minute; 
extern uint32_t second;
extern osMutexId_t mutHour; 
extern osMutexId_t mutMinute; 
extern osMutexId_t mutSecond;

// Semaphores
extern osSemaphoreId_t semIncMinutes;
extern osSemaphoreId_t semIncHours;

//threads
extern osThreadId_t tid_thdDisplayClock;
extern osThreadId_t tid_thdIncSeconds; 
extern osThreadId_t tid_thdIncMinutes; 
extern osThreadId_t tid_thdIncHours; 
int Init_thdDisplayClock (void);
int Init_thdIncSeconds (void);
int Init_thdIncMinutes (void);
int Init_thdIncHours (void);
#endif