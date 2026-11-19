#include "adc.h"

uint32_t samples[200];
uint32_t samples2[100];

void main(){
    adc_init(4);
    adc_start(1);

    while(!adc_done());
   // while(!adc_half_done());
    adc_read(samples, 200);
    print_str("-- ADC data read --");
    
//    while(!adc_done());
//    adc_read(samples2, 100);
//    print_str("-- ADC data read 2 --");

    for (int i = 0; i < 50; i++) 
    {
        print_dec(samples[i]);
    }


}
