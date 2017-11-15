#ifndef _HEADER_SMS_THREADS
#define _HEADER_SMS_THREADS
#include "rtx_os.h"

extern osMemoryPoolId_t mpTextMessage;
extern osMessageQueueId_t mqTextMessage;

extern char* text_message_buffer;
extern uint32_t text_message_buffer_length;

void thdAddTextMessage (void *argument);
#endif
