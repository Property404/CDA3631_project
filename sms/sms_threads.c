#include "sms_threads.h"
#include "sms.h"
#include "string.h"
#include "clock.h"
int Init_thdAddTextMessage (void);
int Init_thdDisplayMessages (void);
osThreadId_t tid_thdDisplayMessages;

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
		char buffer[MAX_TEXT_MESSAGE_LENGTH+1] = {0};
		unsigned int length = 0;
		while(1){
			// Get the next character from the buffer or die
			char character;
			osMessageQueueGet(msgqCharBuffer, &character, NULL, osWaitForever);
			// Only display characters up to Z
			if(character > 'Z'){
				character -= ('z'-'Z');// convert to capital
			}
			buffer[length] = character;
			if(buffer[length] == 0){break;}
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

void thdDisplayMessages(void* argument);
osThreadId_t tid_thdDisplayMessages;
int Init_thdDisplayMessages (void) {
 
  tid_thdDisplayMessages = osThreadNew (thdDisplayMessages, NULL, NULL);
  if (!tid_thdDisplayMessages) return(-1);
  
  return(0);
}
#include "display.h"
#define MAX_LINE_WIDTH 30
#define XOFFSET 15  // horizontal offset from left in characters
#define YOFFSET 12
void thdDisplayMessages(void* argument){
	while(1){
		// No messages, so wait until there are some
		const char* NO_MESSAGES = "NO MESSAGES";
		for(int i=0; NO_MESSAGES[i] != '\0'; i++){
			GLCD_DrawChar((XOFFSET + i+8)*CHARWIDTH, YOFFSET*CHARHEIGHT, NO_MESSAGES[i]);
		}
		osThreadFlagsWait(NEW_MESSAGE, osFlagsWaitAny, osWaitForever);
		osMutexAcquire(mutTextMessageHead, osWaitForever);
		TextMessage* currentMessage = textMessageHead;
		osMutexRelease(mutTextMessageHead);
		while(1){
			// Display current message
			osMutexAcquire(mutTextMessageHead, osWaitForever);
			for(int i=0; i < currentMessage->length; i++){
					char m = currentMessage->message[i];
					if(m<0x20 || m>'Z')break;
					GLCD_DrawChar((XOFFSET + i)*CHARWIDTH, (YOFFSET)*CHARHEIGHT, m);
			}
			osMutexRelease(mutTextMessageHead);
			
			
			// Wait until we get a message from the joystick
			osThreadFlagsWait(NEXT_MESSAGE | PREVIOUS_MESSAGE | SCROLL_UP | SCROLL_DOWN, osFlagsWaitAny, osWaitForever);
		}
	}
}
