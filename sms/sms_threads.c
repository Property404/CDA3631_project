#include "sms.h"
#include "string.h"
#include "clock.h"
#include "timer.h"
#include "display.h"

int Init_thdAddTextMessage (void);
int Init_thdDisplayMessages (void);

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

#define MAX_LINE_WIDTH 30
#define XOFFSET (LCDWIDTH/(2*CHARWIDTH)) // horizontal offset from left in characters into middle
#define YOFFSET (LCDHEIGHT/(2*CHARHEIGHT))  // vertical offset from top into middle

static void displayMessage(const char*const message, const int size, const int y_offset){
	const int X_START_POS = XOFFSET - size/2;
	const int Y_POS = YOFFSET + y_offset;
	for(int i=0;i<size;i++){
		char m = message[i];
		if(m < 0x20 || m > 'Z'){m='?';}//Prefer explicit confusion
		if(m==0)break;
		GLCD_DrawChar((X_START_POS+i)*CHARWIDTH, Y_POS*CHARHEIGHT, message[i]);
	}
}
void thdDisplayMessages(void* argument){
	while(1){
		// No messages, so wait until there are some
		const char* NO_MESSAGES = "NO MESSAGES";
		displayMessage(NO_MESSAGES, 11, -1);
		displayMessage("SORRY", 5, 1);
		osThreadFlagsWait(NEW_MESSAGE, osFlagsWaitAny, osWaitForever);

		osMutexAcquire(mutTextMessageHead, osWaitForever);
		TextMessage* current_message = textMessageHead;
		int scroll_position = 0;
		osMutexRelease(mutTextMessageHead);
		while(1){
			// Clear screen (this is much more fluid than using GLCD_ClearScreen)
			// 	Making a rectangle requires inflating the code size :(
			osMutexAcquire(mutGLCD, osWaitForever);
			for(int i=-2;i<=2;i++){
				for(int j=XOFFSET-MAX_LINE_WIDTH/2;j<XOFFSET+MAX_LINE_WIDTH/2;j++){
					GLCD_DrawChar(j*CHARWIDTH, (YOFFSET+i)*CHARHEIGHT, ' ');
				}
			}

			// Display current text at scroll position
			osMutexAcquire(mutTextMessageHead, osWaitForever);
			int chars_left=current_message->length - (scroll_position*MAX_LINE_WIDTH);
			for(int i=0; i < 2; i++){
				displayMessage(
					current_message->message + ((scroll_position+i)*MAX_LINE_WIDTH),
				chars_left > MAX_LINE_WIDTH ? MAX_LINE_WIDTH : chars_left > 0? chars_left : 0,
				(i-1)*2);
				chars_left -= MAX_LINE_WIDTH;
				if(chars_left<=0)break;
			}
			osMutexRelease(mutGLCD);
			osMutexRelease(mutTextMessageHead);
			

			
			
			// Wait until we get a message from the joystick
			int flags = 
				osThreadFlagsWait(NEXT_MESSAGE | PREVIOUS_MESSAGE | SCROLL_UP | SCROLL_DOWN, osFlagsWaitAny, osWaitForever);
			if (flags & NEXT_MESSAGE){
				
				// Display next in cycle
				osMutexAcquire(mutTextMessageHead, osWaitForever);
				if(current_message->next != NULL){
					current_message = current_message->next;
				}else{
					current_message = textMessageHead;
				}
				osMutexRelease(mutTextMessageHead);
				scroll_position=0;
			}else if(flags & SCROLL_UP){
				if(scroll_position > 0)scroll_position--;
			}else if(flags & SCROLL_DOWN){
				scroll_position++;
			}
			
		}
	}
}
