#ifndef __PORT_H__
#define __PORT_H__

#include <avr/io.h>

#define PORT_MODE_OUT 1
#define PORT_MODE_IN 0

/* 
 * @param port: pointer to port (ex. &DDRB)
 * @param pin: number of pin of given port (ex. DDB1)
 * @param mode: 0 - input, 1 - output
*/
void setPinIOMode(volatile uint8_t* data_register, uint8_t pin, uint8_t mode);

void setPinValue(volatile uint8_t* port, uint8_t pin, uint8_t value);

uint8_t getPinValue(volatile uint8_t port, uint8_t pin);

#endif