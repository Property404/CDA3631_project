#include "sms.h"
#include "clock.h"
#include "string.h"

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
		// Add Text Message struct to memory pool
		TextMessage* text_message = osMemoryPoolAlloc(mplTextMessage, NULL);
		
		// Make a do-while loop here just in case we want to start over
		do{
			
		// Store next 160 or less characters
		text_message->length = 0;
		while(text_message->length < MAX_TEXT_MESSAGE_LENGTH){
			// Get the next character from the buffer or die
			char character;
			osMessageQueueGet(msgqCharBuffer, &character, NULL, osWaitForever);

			
			// Only display characters up to Z
			if(character > 'Z'){
				character -= ('z'-'Z');// convert to capital
			}
			
			// Add char
			text_message->message[text_message->length] = character;
			
			// Break if '\0' or '\r'
			if(character == 0)
				break;
			
			text_message->length++;
		}
		
		// Start over if empty
		if(text_message->length == 0) continue;
		}while(0);
		
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
		text_message->next = NULL;
		text_message->prev = NULL;
		osMutexAcquire(mutTextMessageHead, osWaitForever);
		if(textMessageHead == NULL){
			textMessageHead = text_message;
			// Notify DisplayMessages
			osThreadFlagsSet(tid_thdDisplayMessages, NEW_MESSAGE);
		}else{
			// Find tail and add
			TextMessage* pos = textMessageHead;
			while(pos->next != NULL){pos=pos->next;}
			pos->next = text_message;
			text_message->prev = pos;
		}
		osMutexRelease(mutTextMessageHead);
				
		total_received++;

	}
}
