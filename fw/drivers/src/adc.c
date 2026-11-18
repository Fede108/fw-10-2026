#include "adc.h"

void set_adc_clk_divider(uint32_t clk_div)
{
    REG_ADC_CLK_DIVIDER = clk_div;
}

void adc_start(uint8_t adc_mode)
{
    REG_ADC_START = adc_mode;
}

uint8_t adc_half_done(void)
{
    return REG_ADC_STATUS & 1;
}

uint8_t adc_done(void)
{
    return (REG_ADC_STATUS >> 1) & 1;
}

uint8_t adc_read(uint32_t *data, uint32_t size)
{
    size = (size == 256) ? 256 : size;
    uint8_t i;
    for(i = 0; i<size; i++)
        data[i] = REG_ARRAY_ADC_BUFF[i];
    return i;
}
