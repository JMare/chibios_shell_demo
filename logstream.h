#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#define LOG_STRING_LENGTH 100
#define LOG_STRING_NUMBER 100

// Macro for printing log information
// Using shell console interface
#define XPRINTF(...) {\
    char *_log_string__ = logGetBuffer();\
    if(_log_string__ != NULL){\
        chsnprintf(_log_string__,LOG_STRING_LENGTH,__VA_ARGS__);\
        logSendString(_log_string__);\
    }\
}

void logFIFOInit(void);
char * logGetBuffer(void);
void logSendString(char *s);
char * logGetString(systime_t timeout);
void logReturnString(char *s);

#endif
