#include "hal_timer.h"

static uint32_t timer;

void timer_init(void){
    timer = 0;
}
void  timer_tick(void){
    timer++;
}
uint32_t timer_get_ms(void){
    return timer;
}
