#include "uart.h"

void uart_init(){
    REG_UART0_DATA_FLUSH = 1;
    REG_UART0_DATA_FLUSH = 0;
}

void uart_write_byte(char c){
    REG_UART0_SIZE_TO_TRANSMIT = 1;
    *REG_ARRAY_UART0_BUFF_OUT  = c;
    REG_UART0_START_TRANSMIT   = 1;
    REG_UART0_START_TRANSMIT   = 0;

    while(!REG_UART0_TRANSMISION_DONE);
}

size_t uart_write_str(const char *s)
{
    size_t size = 0;

    while(*s != 0 && size < 256) {
        REG_ARRAY_UART0_BUFF_OUT[size] = *s;
        s++; size++;
    }

    if(size == 0) return 0;

    REG_UART0_SIZE_TO_TRANSMIT = size;
    REG_UART0_START_TRANSMIT   = 1;
    REG_UART0_START_TRANSMIT   = 0;

    while(!REG_UART0_TRANSMISION_DONE);
    
    return size;
}
