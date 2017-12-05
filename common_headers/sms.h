#ifndef COMMON_HEADER_SMS
#define COMMON_HEADER_SMS
#define MAX_TEXT_MESSAGE_LENGTH 160
#define MAX_NUMBER_OF_TEXT_MESSAGES 10
#define CHAR_BUFFER_SIZE (16)
#include "rtx_os.h"

typedef struct{
	// Time message was received
	uint32_t second:8;
	uint32_t minute:8;
	uint32_t hour:8;
	// Cache length(cause strlen() is O(n))
	uint32_t length:8;
	
	// Next in the linked list
	void* next;
	void* prev;
	
	// Message proper
	// Must be an array and not a pointer  in order for it to copy correctly to a memory pool
	char message[MAX_TEXT_MESSAGE_LENGTH+1];
} TextMessage;



// The text message linked list and the mutex
// that protects the WHOLE LIST
extern osMutexId_t mutTextMessageHead;
extern TextMessage* textMessageHead;

// init functions
void initializeSMSObjects(void);
void initializeSMSThreads(void);

// Objs
extern osMemoryPoolId_t mplTextMessage;
extern osMessageQueueId_t msgqCharBuffer;

// Threads
extern osThreadId_t tid_thdDisplayMessages;


// Flags for displaying message
#define NEW_MESSAGE (1<<0) // Used when there are previously no message, but now there are
#define JOYSTICK_RIGHT (1<<1)
#define NEXT_MESSAGE JOYSTICK_RIGHT // Used when joystick right
#define JOYSTICK_LEFT (1<<2)
#define PREVIOUS_MESSAGE JOYSTICK_LEFT // Used when joystick left
#define SCROLL_UP (1<<3) // Move text message up and down when joystick moves as so
#define SCROLL_DOWN (1<<4)
#define JOYSTICK_PUSH (1<<5)
#define DELETE_MESSAGE JOYSTICK_PUSH

// Check for issues
#define _STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]
#define _MEM_POOL_SIZE MAX_NUMBER_OF_TEXT_MESSAGES*sizeof(TextMessage)
#include "RTX_Config.h"
_STATIC_ASSERT(_MEM_POOL_SIZE < OS_MEMPOOL_DATA_SIZE, memory_pools_too_big);
//_STATIC_ASSERT(CHAR_BUFFER_SIZE < OS_MSGQUEUE_DATA_SIZE, message_queue_too_big);
#endif
