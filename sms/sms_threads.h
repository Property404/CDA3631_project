#ifndef _HEADER_SMS_THREADS
#define _HEADER_SMS_THREADS
#include "rtx_os.h"
int Init_thdAddTextMessage (void);
int Init_thdDisplayMessages (void);
osThreadId_t tid_thdDisplayMessages;
#endif
