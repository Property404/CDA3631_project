#include "sms.h"
#include "JOY.h"

void thdPollJoystick(void* argument);
osThreadId_t tid_thdPollJoystick;
int Init_thdPollJoystick (void) {
 
  tid_thdPollJoystick = osThreadNew (thdPollJoystick, NULL, NULL);
  if (!tid_thdPollJoystick) return(-1);
  
  return(0);
}

void thdPollJoystick(void* argument){
	static uint32_t old_status = JOY_CENTER;
	while(1){
		osDelay(1000);
	
		const uint32_t new_status = JOY_GetKeys();
		if(new_status != old_status){
			if(JOY_RIGHT){
				osThreadFlagsSet(tid_thdDisplayMessages, NEXT_MESSAGE);
			}
		}
		
	}
}
