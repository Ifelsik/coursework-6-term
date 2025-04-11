#include "../include/port.h"
#include <assert.h>

void test_port() {
    {  // test
        uint8_t port_d = 0b00100000;
        uint8_t pin_1 = 1;
        
        setPinIOMode(&port_d, pin_1, PORT_MODE_OUT);
        assert(port_d == 0b00100010);
    }
    { // 
        uint8_t port_d = 0b00010001;
        uint8_t pin_4 = 4;

        setPinIOMode(&port_d, pin_4, PORT_MODE_IN);
        assert(port_d == 0b00000001);
    }
}