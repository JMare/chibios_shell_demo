#include "logstream.h"

#include <string.h>

objects_fifo_t log_fifo;
char log_fifo_buffer[LOG_STRING_LENGTH][LOG_STRING_NUMBER];
msg_t log_fifo_msg_buffer[LOG_STRING_NUMBER];

void logFIFOInit(void)
{
    chFifoObjectInit(&log_fifo,LOG_STRING_LENGTH,LOG_STRING_NUMBER,log_fifo_buffer,log_fifo_msg_buffer);
}

// Wrapper for simplicity
char * logGetBuffer(void)
{
    // Obtain a object from the pool if one is free, otherwise S will be null
    return chFifoTakeObjectTimeout(&log_fifo,TIME_IMMEDIATE);
}

// Wrapper for simplicity
void logSendString(char *s)
{
    // Send to shell thread
    chFifoSendObject(&log_fifo,s);
}

// if return pointer not null its a valid string
char * logGetString(systime_t timeout)
{
    char *s;
    msg_t ret = chFifoReceiveObjectTimeout(&log_fifo,(void**)&s,timeout);
    if(ret == MSG_OK)
    {
        // return object to the pool and indicate that we have a valid string
        return s;
    }

    return NULL;
}

// release
void logReturnString(char *s)
{
        chFifoReturnObject(&log_fifo,s);
}
