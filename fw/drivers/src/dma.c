#include "dma.h"

static uint8_t done;

void dma_init(void)
{
   void; 
}

void dma_copy(const void *src, void *dst, uint32_t n_words)
{
    done = 0;

    REG_DMA_SOURCE_ADDR	= src;
    REG_DMA_DEST_ADDR   = dst;
    REG_DMA_SIZE        = n_words;
    REG_DMA_START       = 0;
    REG_DMA_START       = 1;

    dma_is_done();
}

uint8_t dma_is_done(void)
{
    while(!REG_DMA_DONE);
    done = 1;
    return 1;
//    return REG_DMA_DONE;
}
