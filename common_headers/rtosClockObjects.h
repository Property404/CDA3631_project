#ifndef __rtosClockObjects
	#define __rtosClockObjects
	
	// shared variables and mutexes
	extern uint32_t hour; 
	extern uint32_t minute; 
	extern uint32_t second;
	extern osMutexId_t mutHour; 
	extern osMutexId_t mutMinute; 
	extern osMutexId_t mutSecond;
	extern uint32_t timer_minute;
	extern uint32_t timer_second;
	extern uint32_t timer_centisecond;
	extern osMutexId_t mutTimerMinute;
	extern osMutexId_t mutTimerSecond;
	extern osMutexId_t mutTimerCentisecond;

	// Semaphores and flags
	extern osSemaphoreId_t semIncMinutes;
	extern osSemaphoreId_t semIncHours;
	extern osSemaphoreId_t semTimer;
	extern osSemaphoreId_t semIncTimerSeconds;
	
	//threads
	extern osThreadId_t tid_thdDisplayClock;
	extern osThreadId_t tid_thdDisplayTimer;
	extern osThreadId_t tid_thdIncSeconds; 
	extern osThreadId_t tid_thdIncMinutes; 
	extern osThreadId_t tid_thdIncHours; 
	extern osThreadId_t tid_thdIncTimer; 
	extern osThreadId_t tid_thdIncTimerSeconds;
	int Init_thdDisplayClock (void);
	int Init_thdDisplayTimer (void);
	int Init_thdIncSeconds (void);
	int Init_thdIncMinutes (void);
	int Init_thdIncHours (void);
	int Init_thdIncTimer (void);
	int Init_thdIncTimerSeconds (void);
	
	// Update flags for the clock and timer/stopwatch
	#define UPDATE_CENTISECONDS (1<<0) // Only the stopwatch has CENTISECONDS
	#define UPDATE_HOURS UPDATE_CENTISECONDS // Only the clock has hours
	#define UPDATE_SECONDS (1<<1)
	#define UPDATE_MINUTES (1<<2)
	#define UPDATE_COLONS (1<<3)
	#define UPDATE_ALL_TIME_COMPONENTS 0xFF
	
	// Flags for the timer/stopwatch
	#define START_TIMER (1<<0)
	#define STOP_TIMER (1<<1)
	#define RESTART_TIMER (1<<2)
	
#endif