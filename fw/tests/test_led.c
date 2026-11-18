#include "includes.h"
#include "led.h"

void main()
{
    led_on(7);
    print_dec(led_get_status(7));
    led_on(8);
    print_dec(led_get_status(8));
    led_on(5);
    print_dec(led_get_status(5));
    led_on(256);

    print_dec(led_get_status(256));

    print_dec(led_get_status(3));
    led_off(5);
    print_dec(led_get_status(5));
    led_set_mask(0xFF);
    led_write(0xF4);
    led_get_status(2);
    led_toggle(7);
}
