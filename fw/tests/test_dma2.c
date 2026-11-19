#include "includes.h"
#include "dma.h"

uint32_t arr[] = {1, 2 ,3, 4, 5, 6, 7, 8};
uint32_t arr2[8];

void main()
{
    dma_init();
    dma_copy(arr, arr2, 8);
    while(!dma_is_done());

    for(uint8_t i = 0; i<4; i++)
    {
        if(arr[i] != arr2[i]) 
        {
            print_str("error");
            return;
        }
    }
            
    print_str("pass");
}
