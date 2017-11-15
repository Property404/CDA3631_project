#ifndef COMMON_HEADER_SMS
#define COMMON_HEADER_SMS
#define MAX_TEXT_MESSAGE_LENGTH 160
#define MAX_NUMBER_OF_TEXT_MESSAGES 200
#include <stdint.h>

typedef struct{
	// Time message was received
	uint32_t second;
	uint32_t minute;
	uint32_t hour;
	// Cache length(cause strlen() is O(n))
	uint32_t length;
	// Message proper
	// Must be an array and not a pointer  in order for it to copy correctly to a memory pool
	char message[MAX_TEXT_MESSAGE_LENGTH+1];
} TextMessage;

void initializeSMSObjects(void);
void initializeSMSThreads(void);

#endif
