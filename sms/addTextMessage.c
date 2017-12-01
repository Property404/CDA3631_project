#include "sms.h"
#include "clock.h"
#include "string.h"

// This is bigger than the thread stack, so it goes outside
char textBuffer[MAX_TEXT_MESSAGE_LENGTH+1];

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
		
		// Store next 160 or less characters from serial in a buffer

		unsigned int length = 0;
		while(1){
			// Get the next character from the buffer or die
			char character;
			osMessageQueueGet(msgqCharBuffer, &character, NULL, osWaitForever);
			// Only display characters up to Z
			if(character > 'Z'){
				character -= ('z'-'Z');// convert to capital
			}
			textBuffer[length] = character;
			if(textBuffer[length] == 0){break;}
			length++; // Fix plz
			if(length >= MAX_TEXT_MESSAGE_LENGTH){textBuffer[length] = 0; break;}
		}
		
		if(length == 0) continue;

		// Add Text Message struct to memory pool
		TextMessage* text_message = osMemoryPoolAlloc(mplTextMessage, NULL);
		memcpy(text_message->message, textBuffer, length);
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
		text_message->next = NULL;
		osMutexAcquire(mutTextMessageHead, osWaitForever);
		if(textMessageHead == NULL){
			textMessageHead = text_message;
			// Notify DisplayMessages
			osThreadFlagsSet(tid_thdDisplayMessages, NEW_MESSAGE);
		}else{
			TextMessage* pos = textMessageHead;
			while(pos->next != NULL){pos=pos->next;}
			pos->next = text_message;
		}
		osMutexRelease(mutTextMessageHead);
				
		total_received++;

	}
}
