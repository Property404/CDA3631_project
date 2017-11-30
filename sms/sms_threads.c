#include "sms_threads.h"
#include "sms.h"
#include "string.h"
#include "clock.h"
#include "Board_GLCD.h"

// Add a text message to the msgq
void thdAddTextMessage(void* argument);
osThreadId_t tid_thdAddTextMessage;
int Init_thdAddTextMessage (void) {
 
  tid_thdAddTextMessage = osThreadNew (thdAddTextMessage, NULL, NULL);
  if (!tid_thdAddTextMessage) return(-1);
  
  return(0);
}

void thdAddTextMessage(void* argument){
	uint32_t total_received = 0;
	while(1){
		GLCD_DrawChar(10*15, 4*24, '0'+(char)total_received);
		
		// Store next 160 or less characters from serial in a buffer
		char buffer[MAX_TEXT_MESSAGE_LENGTH+1] = {0};
		unsigned int length = 0;
		while(1){
			// Get the next character from the buffer or die
			char character;
			osMessageQueueGet(msgqCharBuffer, &character, NULL, osWaitForever);
			buffer[length] = character;
			if(buffer[length] == 0 || buffer[length]=='a'){break;}
			length++; // Fix plz
			if(length >= MAX_TEXT_MESSAGE_LENGTH){buffer[length] = 0; break;}
		}
		
		if(length == 0) continue;

		// Add Text Message struct to memory pool
		TextMessage* text_message = osMemoryPoolAlloc(mplTextMessage, NULL);
		memcpy(text_message->message, buffer, length);
		text_message->length = length;
		
		// Have to add the time
		osMutexAcquire(mutSecond, osWaitForever);
		text_message->second = second;
		osMutexRelease(mutSecond);
		osMutexAcquire(mutMinute, osWaitForever);
		text_message->minute = minute;
		osMutexRelease(mutMinute);
		osMutexAcquire(mutHour, osWaitForever);
		text_message->hour = hour;
		osMutexRelease(mutHour);
	
		// Add to linked list
		/* to do */
		
		total_received++;

	}
}

