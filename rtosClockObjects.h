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
	extern uint32_t timer_millisecond;
	extern osMutexId_t mutTimerMinute;
	extern osMutexId_t mutTimerSecond;
	extern osMutexId_t mutTimerMillisecond;
	extern osSemaphoreId_t semIncMinutes;
	extern osSemaphoreId_t semIncHours;
	
	//threads
	extern osThreadId_t tid_thdDisplayClock;
	extern osThreadId_t tid_thdDisplayTimer;
	extern osThreadId_t tid_thdIncSeconds; 
	extern osThreadId_t tid_thdIncMinutes; 
	extern osThreadId_t tid_thdIncHours; 
	int Init_thdDisplayClock (void);
	int Init_thdDisplayTimer (void);
	int Init_thdIncSeconds (void);
	int Init_thdIncMinutes (void);
	int Init_thdIncHours (void);
	
	// Update flags for the clock and timer/stopwatch
	#define UPDATE_MILLISECONDS (1<<0) // Only the stopwatch has milliseconds
	#define UPDATE_HOURS UPDATE_MILLISECONDS // Only the clock has hours
	#define UPDATE_SECONDS (1<<1)
	#define UPDATE_MINUTES (1<<2)
	#define UPDATE_COLONS (1<<3)
	
	// Flags for the timer/stopwatch
	#define START_TIMER (1<<0)
	#define STOP_TIMER (1<<1)
	#define RESTART_TIMER (1<<2)
	
#endif
