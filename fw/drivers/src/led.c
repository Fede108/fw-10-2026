#include "led.h"

static uint32_t mask_value;

void led_init(void)
{
    REG_LED = 0;
}

void led_on(uint8_t idx)
{
    uint8_t prev = 0;
    uint8_t new = 0;
    new = 1 << idx;
  //  if(REG_LED & ~mask_value){
        prev = REG_LED;
        REG_LED = prev | new;
   // }
}

void led_off(uint8_t idx)
{
    uint8_t prev = 0;
    uint8_t new = 0;
    new = 1 << idx;
  //  if(REG_LED & ~mask_value){
        prev = REG_LED;
        REG_LED = prev & ~new;
  //  }
}

uint8_t led_get_status(uint8_t idx)
{
    uint8_t status = 0;
//    if(idx < 8){
        status = REG_LED;
        status = status >> idx;
  //  }

    return status & ~0xFE;
}

void led_toggle(uint8_t idx)
{
    if(led_get_status(idx)) 
        led_off(idx);
    else
        led_on(idx);
}

void led_write(uint8_t value)
{
    uint8_t prev = 0;
    prev = REG_LED;
//    REG_LED = prev | (value & mask_value);
    REG_LED = prev | value ;
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
