/* This header provides display methods to both the stopwatch and clock
 */
#ifndef COMMON_HEADER_DISPLAY_TIME_COMPONENTS
#define COMMON_HEADER_DISPLAY_TIME_COMPONENTS
#include "time_components.h"
#include "display.h"

#define XOFFSET 23  // time horizontal offset from left in characters


// Make these functions `static inline`, cause it's kinda silly to
// have a separate C file

// Display a two-digit numeric component of the clock or timer
static inline void drawNumber(uint32_t number, uint32_t char_offset, double y_offset){
	GLCD_DrawChar((XOFFSET+char_offset)*CHARWIDTH,y_offset*CHARHEIGHT, 0x30 + number/10);
	GLCD_DrawChar((XOFFSET+char_offset+1)*CHARWIDTH,y_offset*CHARHEIGHT, 0x30 + number%10);
}
// Display the colons between the numeric components
static inline void drawColons(double y_offset){
	GLCD_DrawChar((XOFFSET+2)*CHARWIDTH,y_offset*CHARHEIGHT, ':');
	GLCD_DrawChar((XOFFSET+5)*CHARWIDTH,y_offset*CHARHEIGHT, ':');
}

#undef XOFFSET
#undef LCDWIDTH
#undef LCDHEIGHT
#undef CHARWIDTH
#undef CHARHEIGHT
#endif
