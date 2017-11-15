#include "sms_threads.h"
#include "sms.h"
#include "string.h"
#include "clock.h"

// Declare objects here, because they're private to these threads
// Wrap all objects manipulation in small functions in sms.c
osMemoryPoolId_t mpTextMessage;
osMessageQueueId_t mqTextMessage;
osSemaphoreId_t semAddTextMessage;
osMemoryPoolId_t mpCharBuffer;
osMessageQueueId_t mqCharBuffer;

// Add a text message to the queue
osThreadId_t tid_thdAddTextMessage;
int Init_thdAddTextMessage (void) {
 
  tid_thdAddTextMessage = osThreadNew (thdAddTextMessage, NULL, NULL);
  if (!tid_thdAddTextMessage) return(-1);
  
  return(0);
}

void thdAddTextMessage(void* argument){
	while(1){
		
		// Store next 160 or less characters from serial in a buffer
		char buffer[MAX_TEXT_MESSAGE_LENGTH+1] = {0};
		unsigned int length = 0;
		while(1){
			// Get the next character from the buffer or die
			if(osMessageQueueGet(mqCharBuffer, (buffer+length), NULL, osWaitForever) != osOK) while(1);
			if(buffer[length] == 0){break;}
			length++; // Fix plz
			if(length == MAX_TEXT_MESSAGE_LENGTH){buffer[length] = 0; break}
		}
		// Cause length is not "zero-indexed"
		length++;
		
		// Convert to a Text Message struct
		TextMessage text_message = osMemoryPoolAlloc(mplTxt;
		memcpy(text_message.message, buffer, length);
		text_message.length = length;
		// Have to add the time
		osMutexAcquire(mutSecond, osWaitForever);
		text_message.second = second;
		osMutexRelease(mutSecond);
		osMutexAcquire(mutMinute, osWaitForever);
		text_message.minute = minute;
		osMutexRelease(mutMinute);
		osMutexAcquire(mutHour, osWaitForever);
		text_message.hour = hour;
		osMutexRelease(mutHour);
		
		// Add to memory pool
		

	}
}

