#include "sms.h"
#include "sms_threads.h"
#include "rtos_util.h"
osMemoryPoolId_t mplTextMessage;
osMemoryPoolId_t mplCharBuffer;
osMessageQueueId_t msgqCharBuffer;
osMutexId_t mutTextMessageHead;
TextMessage* textMessageHead;

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
}
