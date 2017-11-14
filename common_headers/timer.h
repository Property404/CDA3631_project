#ifndef COMMON_HEADER_TIMER
#define COMMON_HEADER_TIMER
#include "rtx_os.h"
#include "time_components.h"
/* This header provides definitions relating exclusively to the timer */
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
extern osSemaphoreId_t semIncTimerMinutes;

//threads
extern osThreadId_t tid_thdDisplayTimer;
extern osThreadId_t tid_thdIncTimer; 
extern osThreadId_t tid_thdIncTimerSeconds;
extern osThreadId_t tid_thdIncTimerMinutes;

int Init_thdDisplayTimer (void);
int Init_thdIncTimer (void);
int Init_thdIncTimerSeconds (void);
int Init_thdIncTimerMinutes(void);


// Flags for the timer/stopwatch
#define START_TIMER (1<<0)
#define STOP_TIMER (1<<1)
#define RESTART_TIMER (1<<2)

// Init methods
void initializeTimerThreads(void);
void initializeTimerObjects(void);

#endif
