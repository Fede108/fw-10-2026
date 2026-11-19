#include "adc.h"

void adc_init(uint32_t clk_div)
{
    REG_ADC_CLK_DIVIDER = clk_div;
}

void adc_start(uint8_t adc_mode)
{
    REG_ADC_START = 0;
    REG_ADC_START = adc_mode << 1;
    REG_ADC_START = REG_ADC_START | 1;
}

uint8_t adc_half_done(void)
{
    while(!(REG_ADC_STATUS & 1));
    return 1;
}

uint8_t adc_done(void)
{
    while(!((REG_ADC_STATUS >> 1) & 1));
    return 1;
}

uint16_t adc_read(uint32_t *data, uint32_t size)
{
    if(data == 0) return 0;
    size = (size > 256) ? 256 : size;

    adc_start(0);
    adc_done();

    uint16_t i;
    for(i = 0; i<size; i++)
        data[i] = REG_ARRAY_ADC_BUFF[i];
    return i;
}
