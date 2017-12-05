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
		if(m==0)break;
		if(m < 0x20 || m > 'Z'){m='?';}//Prefer explicit confusion

		GLCD_DrawChar((X_START_POS+i)*CHARWIDTH, Y_POS*CHARHEIGHT, message[i]);
	}
}
void thdDisplayMessages(void* argument){
	while(1){
		// No messages, so wait until there are some
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
			// Displaying three lines
			for(int i=0; i <= 2; i++){
				displayMessage(
					// Actual message at the proper position
					current_message->message + ((scroll_position+i)*MAX_LINE_WIDTH),
				
					chars_left > MAX_LINE_WIDTH ? MAX_LINE_WIDTH : chars_left > 0? chars_left : 0,
					
					// We're using double spacing here
					(i-1)*2);
				chars_left -= MAX_LINE_WIDTH;
				if(chars_left<=0)break;
			}
			osMutexRelease(mutGLCD);
			osMutexRelease(mutTextMessageHead);
			
			
			// Wait until we get a message from the joystick
			const int flags = 
				osThreadFlagsWait(DELETE_MESSAGE | NEXT_MESSAGE | PREVIOUS_MESSAGE | SCROLL_UP | SCROLL_DOWN, osFlagsWaitAny, osWaitForever);
			
			scroll_position=0;// Have to reset scroll position, otherwise we'd start in random or invalid place
			
			if (flags & NEXT_MESSAGE){
				
				// Display next in cycle
				osMutexAcquire(mutTextMessageHead, osWaitForever);
				if(current_message->next != NULL){
					current_message = current_message->next;
				}else{
					current_message = textMessageHead;
				}
				osMutexRelease(mutTextMessageHead);
			
			}else if(flags & PREVIOUS_MESSAGE){
				// Display previous in cycle
				
				osMutexAcquire(mutTextMessageHead, osWaitForever);
				
				if(current_message->prev != NULL){
					current_message = current_message->prev;
				}else{
					while(current_message->next != NULL)current_message = current_message->next;
				}
				
				
				osMutexRelease(mutTextMessageHead);
			}else if(flags & DELETE_MESSAGE){
				// Deletion involves removing from linked list
				// And possibly exiting this loop if there are no 
				// messages left(to go back to the "no messages screen"
				//
				// But, in addition, we must prompt the user first
				osMutexAcquire(mutGLCD, osWaitForever);
				
				// Clear text		
				for(int i=-2;i<=2;i++){
					for(int j=XOFFSET-MAX_LINE_WIDTH/2;j<XOFFSET+MAX_LINE_WIDTH/2;j++){
						GLCD_DrawChar(j*CHARWIDTH, (YOFFSET+i)*CHARHEIGHT, ' ');
					}
				}
				
				// Constants for positions in dialog
				const float RECTANGLE_SIZE = .9;//Between 0 and 1
				const int RECTANGLE_X = LCDWIDTH*(1-RECTANGLE_SIZE);
				const int RECTANGLE_WIDTH = LCDWIDTH*(RECTANGLE_SIZE)-RECTANGLE_X;
				const int RECTANGLE_Y = LCDHEIGHT*(1-RECTANGLE_SIZE);
				const int RECTANGLE_HEIGHT = LCDHEIGHT*(RECTANGLE_SIZE)-RECTANGLE_Y;
				
				// Position for the "No" and "Yes" options
				const int NO_POS_X = RECTANGLE_WIDTH*.75 + RECTANGLE_X;
				const int NO_POS_Y = RECTANGLE_Y+.75*RECTANGLE_HEIGHT;//-RECTANGLE_Y1*.25 + RECTANGLE_Y2;
				const int YES_POS_X = RECTANGLE_WIDTH*.25 + RECTANGLE_X;
				const int YES_POS_Y = NO_POS_Y;
				
				// Draw the dialog
				displayMessage("DELETE?", 7, 0);

				osMutexRelease(mutGLCD);
				// Now, wait for a user response
				uint8_t pos = 0;// 0 if the "yes" is selected, 1 if "no" is
				while(1){
					osMutexAcquire(mutGLCD, osWaitForever);
					// De-highlight unselected item, and highlight the selected
					if(pos==0){
						GLCD_SetForegroundColor(SECONDARY_FOREGROUND_COLOR);
					}
					GLCD_DrawString(NO_POS_X, NO_POS_Y, "NO");
					
					
					if(pos==1){
						GLCD_SetForegroundColor(SECONDARY_FOREGROUND_COLOR);
					}else{
						GLCD_SetForegroundColor(FOREGROUND_COLOR);
					}
					GLCD_DrawString(YES_POS_X, YES_POS_Y, "YES");
					GLCD_SetForegroundColor(FOREGROUND_COLOR);
					osMutexRelease(mutGLCD);
					
					// Allow user to switch between and selecto ptions
					const int flags = osThreadFlagsWait(JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_PUSH, osFlagsWaitAny, osWaitForever);
					
					if(flags & JOYSTICK_LEFT && pos){
						pos = 0;// Select yes
					}else
					if(flags & JOYSTICK_RIGHT && !pos){
						pos = 1;//select no
					}else
					
					if(flags & JOYSTICK_PUSH){
						
						// Clear dialog
						GLCD_SetForegroundColor(BACKGROUND_COLOR);
						GLCD_DrawString(YES_POS_X, YES_POS_Y, "YES");
						GLCD_DrawString(NO_POS_X, NO_POS_Y, "NO");
						displayMessage("DELETE?", 7, 0);
						GLCD_SetForegroundColor(FOREGROUND_COLOR);
					
						// Delete if "yes" selected
						if(pos==0){
							osMutexAcquire(mutTextMessageHead, osWaitForever);
							// Record it's links
							TextMessage* next = current_message->next;
							TextMessage* prev = current_message->prev;
							
							// Now, we deallocate
							int is_head = 0;
							if(current_message == textMessageHead){
								is_head = 1;
							}
							osMemoryPoolFree(mplTextMessage, current_message);
							
							// Change links
							next->prev = prev;
							prev->next = next;
							
							// Change current message
							current_message = next==NULL?prev:next;
							if(is_head){
								textMessageHead = current_message;
							}
							osMutexRelease(mutTextMessageHead);
							
							
						}
						break;
					}
					
				}
				
				// If none are left, go back to "no messages" screen
				osMutexAcquire(mutTextMessageHead, osWaitForever);
				if(textMessageHead == NULL){osMutexRelease(mutTextMessageHead);break;}
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
