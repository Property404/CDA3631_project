#include "sms.h"
#include "JOY.h"

void thdPollJoystick(void* argument);
osThreadId_t tid_thdPollJoystick;
int Init_thdPollJoystick (void) {
 
  tid_thdPollJoystick = osThreadNew (thdPollJoystick, NULL, NULL);
  if (!tid_thdPollJoystick) return(-1);
  
  return(0);
}

#define TICKS_TO_WAIT 100
// Polls the joystick every {some amount}ticks
// and delivers a message to thdDisplayMessage
// IF the joystick position has changed
void thdPollJoystick(void* argument){
	// Keep track of what the last joystick
	// position was
	uint32_t old_status = 0;
	
	// Check every {amount} ticks
	while(1){
		osDelay(TICKS_TO_WAIT);
	
		// Get keys and deliver message if applicable
		const uint32_t new_status = JOY_GetKeys();
		if(new_status != old_status){
			uint32_t flag = 0;
			
			if(new_status & JOY_RIGHT){
				flag = NEXT_MESSAGE;
			}else if(new_status & JOY_LEFT){
				flag = PREVIOUS_MESSAGE;
			}else if(new_status & JOY_UP){
				flag = SCROLL_UP;
			}else if(new_status & JOY_DOWN){
				flag = SCROLL_DOWN;
			}
			else if(new_status & JOY_CENTER){
				flag = DELETE_MESSAGE;
			}
			osThreadFlagsSet(tid_thdDisplayMessages, flag);
			old_status = new_status;
		}
		
		
		
	}
}
