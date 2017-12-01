#include "sms.h"
#include "rtos_util.h"

// Define all objects relating to SMS here
osMemoryPoolId_t mplTextMessage;
osMemoryPoolId_t mplCharBuffer;
osMessageQueueId_t msgqCharBuffer;
osMutexId_t mutTextMessageHead;
TextMessage* textMessageHead;
osSemaphoreId_t semJoystick;

int Init_thdAddTextMessage (void);
int Init_thdDisplayMessages (void);

void initializeSMSThreads(void){
	Init_thdAddTextMessage();
	Init_thdDisplayMessages();
}

void initializeSMSObjects(void){
		msgqCharBuffer = newMessageQueueOrDie(CHAR_BUFFER_SIZE, sizeof(char));
	  mplTextMessage = newMemoryPoolOrDie(MAX_NUMBER_OF_TEXT_MESSAGES, sizeof(TextMessage));
		mutTextMessageHead = newMutexOrDie();
		textMessageHead = NULL;
		semJoystick = newSemaphoreOrDie(1,0);
}
