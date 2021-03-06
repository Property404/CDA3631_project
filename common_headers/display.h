#ifndef COMMON_HEADER_DISPLAY
#define COMMON_HEADER_DISPLAY
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#define LCDWIDTH 320
#define LCDHEIGHT 240
#define CHARWIDTH 6
#define CHARHEIGHT 8
#define BACKGROUND_COLOR GLCD_COLOR_BLACK
#define FOREGROUND_COLOR GLCD_COLOR_GREEN
#define SECONDARY_FOREGROUND_COLOR GLCD_COLOR_DARK_GREEN

#include "rtx_os.h"
// Only one thread can access the GLCD at a time
extern osMutexId_t mutGLCD;

void setupDisplay(void);
void drawNumber(const uint32_t number, const uint32_t char_offset, const float y_offset);
void drawColons(const float y_offset);
#endif
