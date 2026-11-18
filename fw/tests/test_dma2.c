#include "includes.h"
#include "dma.h"

uint32_t arr[] = {1, 2 ,3, 4, 5, 6, 7, 8};
uint32_t arr2[4];

uint32_t arr3[] = {1, 2 ,3, 4, 5, 6, 7, 8};

void main()
{
    dma_init();
    dma_copy(arr, arr+4, 4);
    while(!dma_is_done());

    for(uint8_t i = 0; i<4; i++)
    {
        if(arr[i] != arr[i+4]) 
        {
            print_str("error");
            return;
        }
    }
            
    print_str("pass");
}
