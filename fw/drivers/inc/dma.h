#pragma once

#include "includes.h"   
#include <stdint.h>

void dma_init(void);
int dma_copy(const void *src, void *dst, uint32_t n_words);
uint8_t dma_is_done(void);

