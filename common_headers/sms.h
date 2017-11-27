#ifndef COMMON_HEADER_SMS
#define COMMON_HEADER_SMS
#define MAX_TEXT_MESSAGE_LENGTH 160
#define MAX_NUMBER_OF_TEXT_MESSAGES 10
#define CHAR_BUFFER_SIZE MAX_TEXT_MESSAGE_LENGTH


#include "rtx_os.h"

typedef struct{
	// Time message was received
	uint32_t second:8;
	uint32_t minute:8;
	uint32_t hour:8;
	// Cache length(cause strlen() is O(n))
	uint32_t length:8;
	// Message proper
	// Must be an array and not a pointer  in order for it to copy correctly to a memory pool
	char message[MAX_TEXT_MESSAGE_LENGTH+1];
} TextMessage;

void initializeSMSObjects(void);
void initializeSMSThreads(void);

extern osMemoryPoolId_t mplTextMessage;
extern osMemoryPoolId_t mplCharBuffer;
extern osMessageQueueId_t msgqCharBuffer;

// Check for issues
/*
#define _STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]
#define _MEM_POOL_SIZE MAX_NUMBER_OF_TEXT_MESSAGES*sizeof(TextMessage) + CHAR_BUFFER_SIZE*sizeof(char)
#include "RTX_Config.h"
_STATIC_ASSERT(_MEM_POOL_SIZE < OS_MEMPOOL_DATA_SIZE, memory_pools_too_big);
*/
#endif
