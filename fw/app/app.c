#include "includes.h"
#include "led.h"
#include "dma.h"
#include "uart1.h"
#include "adc.h"

#define N 200

static uint32_t src_arr[N];
static uint32_t dst_arr[N];


void main()
{
    uint8_t flag = 1;

    set_adc_clk_divider(4);
    adc_start(1);

    led_on(1);
    while(!adc_half_done());

    uart_write_str("mitad lectura adc completada");
    adc_read(src_arr, N/2);   
    
    led_on(2);
    dma_copy(src_arr, dst_arr, N/2);   

    while(!adc_done());

    adc_read(src_arr + N/2, N/2);   
    
    dma_copy(src_arr, dst_arr, N/2);   

    uart_write_str("mitad transf dma completada");

    led_set_mask(0x00);
    for(uint8_t i=0; i<N; i++){

        led_set_mask(dst_arr[i] >> 24);

        if(dst_arr[i] > 0xFFFFF && flag){
            flag = 0;
            uart_write_str("señal supera 2.4v");
        } 
        else if(!flag && dst_arr[i] < 0xFFFF) {
            uart_write_str("señal debajo 1.6v");
            flag = 1;
        }
//            led_set_mask(0xFF);
//        if(dst_arr[i] > 0xFFFF)
//            led_set_mask(0xF);
//        else
//            led_set_mask(0x0);

        if(src_arr[i] != dst_arr[i]){
            uart_write_str("error en dma");
            break;
        }
    }
}

