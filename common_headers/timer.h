#ifndef COMMON_HEADER_TIMER
#define COMMON_HEADER_TIMER
#include "time_components.h"
// shared variables and mutexes
extern uint32_t timer_minute;
extern uint32_t timer_second;
extern uint32_t timer_centisecond;
extern osMutexId_t mutTimerMinute;
extern osMutexId_t mutTimerSecond;
extern osMutexId_t mutTimerCentisecond;

// Semaphores and flags
extern osSemaphoreId_t semTimer;
extern osSemaphoreId_t semIncTimerSeconds;

//threads
extern osThreadId_t tid_thdDisplayTimer;
extern osThreadId_t tid_thdIncTimer; 
extern osThreadId_t tid_thdIncTimerSeconds;

int Init_thdDisplayTimer (void);
int Init_thdIncTimer (void);
int Init_thdIncTimerSeconds (void);


// Flags for the timer/stopwatch
#define START_TIMER (1<<0)
#define STOP_TIMER (1<<1)
#define RESTART_TIMER (1<<2)

#endif
