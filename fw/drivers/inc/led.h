#pragma once

#include "includes.h"      /* config.h:  REG_LED = 0x_0004_0038 */
#include <stdint.h>

void led_init(void);
void led_on(uint8_t idx);
void led_off(uint8_t idx);
void led_toggle(uint8_t idx);
void led_write(uint8_t value);
void led_set_mask(uint32_t mask);
uint32_t led_get_mask(void);
uint8_t led_get_status(uint8_t idx);
