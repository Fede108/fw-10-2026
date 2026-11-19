#include "includes.h"
#include "led.h"

void main()
{
    led_on(7);
    print_dec(led_get_status(7));
    led_on(8);
    print_dec(led_get_status(8));
    
    led_on(10);
    print_dec(led_get_status(10));
    
    led_on(5);
    led_off(5);
    print_dec(led_get_status(5));

    led_set_mask(0xF);
    led_write(0xF4);

    led_toggle(7);
    print_dec(led_get_status(7));
}
