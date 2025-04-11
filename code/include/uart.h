#ifndef __UART_H__
#define __UART_H__

#include <avr/io.h>
#include <stdarg.h>
#include <stdio.h>

void uart_init(uint32_t f_cpu, uint32_t baud);

void uart_transmit(uint8_t data);

uint8_t uart_receive();

void uart_puts(const char* string);

void uart_printf(const char* fmt, ...);

#endif