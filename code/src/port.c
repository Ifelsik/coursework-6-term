#include "../include/port.h"

void setPinIOMode(volatile uint8_t* data_register, uint8_t pin, uint8_t mode) {
    if (mode) {
        *data_register |= (1 << pin);
    } else {
        *data_register &= ~(1 << pin);
    }
}

void setPinValue(volatile uint8_t* port, uint8_t pin, uint8_t value) {
    if (value) {
        *port |= (1 << pin);
    } else {
        *port &= ~(1 << pin);
    }
}

uint8_t getPinValue(volatile uint8_t port, uint8_t pin) {
    return (port >> pin) & 1;
}