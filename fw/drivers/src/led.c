#include "led.h"

static uint32_t mask_value;

void led_init(void)
{
    REG_LED = 0;
}

int led_on(uint8_t idx)
{
    uint8_t new = 0;
    if(idx>7) return -1;
    new = 1 << idx;
    REG_LED = REG_LED | new;
    return 0;
}

int led_off(uint8_t idx)
{
    uint8_t new = 0;
    if(idx>7) return -1;
    new = 1 << idx;
    REG_LED = REG_LED & ~new;
    return 0;
}

uint8_t led_get_status(uint8_t idx)
{
    uint8_t status = 0;
    status = REG_LED >> idx;
    return status & 1;
}

int led_toggle(uint8_t idx)
{
    if(led_get_status(idx)) 
        return led_off(idx);
    else
        return led_on(idx);
}

void led_write(uint8_t value)
{
//    REG_LED = value & mask_value;
    REG_LED = value ;
}

void led_set_mask(uint32_t mask)
{
    REG_LED = mask;
    //mask_value = mask;
}

uint32_t led_get_mask(void)
{
    return REG_LED;
}
