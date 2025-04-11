#include "../include/uart.h"

void uart_init(uint32_t f_cpu, uint32_t baud) {
    uint32_t ubrr_value = f_cpu / (16UL * baud) - 1;

    UBRR0H = (uint8_t) (ubrr_value >> 8);
    UBRR0L = (uint8_t) (ubrr_value & 0xFF);

    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))) {}
    UDR0 = data;
}

uint8_t uart_receive() {
    while (!(UCSR0A & (1 << RXC0))) {}
    return UDR0;
}

void uart_puts(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

void uart_printf(const char *fmt, ...) {
    char buf[64];  // default buffer size
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    uart_puts(buf);
}