#include "uart.h"
#include "includes.h"

void main(){

//    uart_init();
    char c = 'A';
    uart_write_byte(c);
    char *s = "abcde";
    uart_write_str(s);

}
