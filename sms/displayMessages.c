#include "sms.h"
#include "display.h"

void thdDisplayMessages(void* argument);
osThreadId_t tid_thdDisplayMessages;
int Init_thdDisplayMessages (void) {
 
  tid_thdDisplayMessages = osThreadNew (thdDisplayMessages, NULL, NULL);
  if (!tid_thdDisplayMessages) return(-1);
  
  return(0);
}

#define MAX_LINE_WIDTH 16
#define XOFFSET (LCDWIDTH/(2*CHARWIDTH)) // horizontal offset from left in characters into middle
#define YOFFSET (LCDHEIGHT/(2*CHARHEIGHT))  // vertical offset from top into middle

// This displays a line in the centerish of the screen
// It's VERY fancy
// message - the message to display
// size - size of the message
// y_offset - how many units from the center the message is, usually from [-2, 2]
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
		displayMessage("NO MSGS", 7, 0);
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
			const int flags = 
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
				scroll_position=0;// Have to reset scroll position, otherwise we'd start in random or invalid place
			
			}else if(flags & PREVIOUS_MESSAGE){
				// Display previous is cycle
				// This is tricker because we have to start from the head
				// since it's not a doubly linked list
				// Plus, this is O(n) worst case and average case
				
				osMutexAcquire(mutTextMessageHead, osWaitForever);
				
				// If there's only one item in LL, just skip all this nonsense
				if(textMessageHead->next == NULL){osMutexRelease(mutTextMessageHead);continue;}
				
				// Cycle through to find out which TM points to the current one
				TextMessage* tm = textMessageHead;
				while(tm -> next != current_message){
					// If tm->next is NULL, that means we were at the head, so we're picking the last tail
					if(tm->next == NULL)break;
					
					tm = tm->next;
				};
				current_message = tm;
				
				osMutexRelease(mutTextMessageHead);
				
			// Change where we are in scroll
			}else if(flags & SCROLL_UP){
				if(scroll_position > 0)
					scroll_position--;
			}else if(flags & SCROLL_DOWN){
				if(scroll_position < current_message->length/MAX_LINE_WIDTH)
					scroll_position++;
			}
			
		}
	}
}
