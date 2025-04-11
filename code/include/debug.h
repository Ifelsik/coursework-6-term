#ifndef __AVR_DEBUG_H__
#define __AVR_DEBUG_H__

//consider define DEBUG as a flag -DDEBUG in Compilers Flag

#ifdef DEBUG
    #include "uart.h"
    #define DEBUG_PRINT(fmt, ...) do { uart_printf("[DBG] " fmt, ##__VA_ARGS__); } while(0)
#else 
    #define DEBUG_PRINT(fmt, ...) do { } while(0)
#endif

#endif