#pragma once

#include "includes.h"   
#include <stdint.h>

void adc_init(uint32_t clk_div);
void adc_start(uint8_t adc_mode);
uint8_t adc_half_done(void);
uint8_t adc_done(void);
uint16_t adc_read(uint32_t *data, uint32_t size);
