#include "dma.h"

void dma_init(void)
{
   void; 
}

int dma_copy(const void *src, void *dst, uint32_t n_words)
{
    if(src == dst) return -1;

    if(((uint32_t*)src + n_words) > 0x0005FFFF) return -1;
    if(((uint32_t*)dst + n_words) > 0x0005FFFF) return -1;

    REG_DMA_SOURCE_ADDR	= src;
    REG_DMA_DEST_ADDR   = dst;
    REG_DMA_SIZE        = n_words;
    REG_DMA_START       = 0;
    REG_DMA_START       = 1;

    while(!dma_is_done());

    return 0;
}

uint8_t dma_is_done(void)
{
    return REG_DMA_DONE;
}
