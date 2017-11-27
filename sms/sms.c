#include "sms.h"
#include "sms_threads.h"
#include "rtos_util.h"
osMemoryPoolId_t mplTextMessage;
osMemoryPoolId_t mplCharBuffer;
osMessageQueueId_t msgqCharBuffer;

int Init_thdAddTextMessage (void);
void initializeSMSThreads(void){
	Init_thdAddTextMessage();
}
void initializeSMSObjects(void){
	mplTextMessage = newMemoryPoolOrDie(MAX_NUMBER_OF_TEXT_MESSAGES, sizeof(TextMessage));
	msgqCharBuffer = newMessageQueueOrDie(CHAR_BUFFER_SIZE, sizeof(char));
	mplCharBuffer  = newMemoryPoolOrDie(CHAR_BUFFER_SIZE, sizeof(char));
}
