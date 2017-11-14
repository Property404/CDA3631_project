#ifndef COMMON_HEADER_TIME_COMPONENTS
#define COMMON_HEADER_TIME_COMPONENTS
/* This header provides definitions common to both the timer and clock */

// Update flags for the clock and timer/stopwatch
#define UPDATE_CENTISECONDS (1<<0) // Only the stopwatch has CENTISECONDS
#define UPDATE_HOURS UPDATE_CENTISECONDS // Only the clock has hours
#define UPDATE_SECONDS (1<<1)
#define UPDATE_MINUTES (1<<2)
#define UPDATE_COLONS (1<<3)
#define UPDATE_ALL_TIME_COMPONENTS 0xFF
#endif
