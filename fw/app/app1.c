#include "led.h"
#include "dma.h"
#include "uart1.h"
#include "adc.h"

#define N 200
#define UMBRAL_2_4V 0xBA2E8BA2 
#define UMBRAL_1_6V 0x7C1F07C1

static uint32_t src_arr[N];
static uint32_t dst_arr[N];

void main()
{
    uint8_t flag = 1;

    adc_init(4);
    adc_start(1);
    uart_write_str("start adc");
    led_on(0);
    while(!adc_done());

    adc_read(src_arr, N);   
    uart_write_str("lectura adc completada");
    led_off(0);

    uart_write_str("start dma");
    led_set_mask(0x02);
    dma_copy(src_arr, dst_arr, N);   
    uart_write_str("transf dma completada");
    led_set_mask(0x00);

    for(uint8_t i=0; i<N; i++){

    //    led_set_mask(dst_arr[i] >> 24);

        if(dst_arr[i] > UMBRAL_2_4V && flag){
            flag = 0;
            uart_write_str("señal supera 2.4v");
        } 
        else if(!flag && dst_arr[i] < UMBRAL_1_6V) {
            uart_write_str("señal debajo 1.6v");
            flag = 1;
        }
        if(src_arr[i] != dst_arr[i]){
            uart_write_str("error en dma");
            break;
        }
    }
}
