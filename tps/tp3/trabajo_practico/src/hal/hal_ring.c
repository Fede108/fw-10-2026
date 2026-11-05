#include "hal_ring.h"
#include <stddef.h>

void ring_init(ring_buf_t *rb)
{
   for(size_t i = 0; i<sizeof(*rb); i++)
        *(((uint8_t *)rb) + i) = 0;

}

int  ring_push(ring_buf_t *rb, uint8_t byte)
{
    if(ring_is_full(rb)) return -1; 
    rb->data[rb->head] = byte;
    rb->head = (rb->head + 1) % RING_BUF_SIZE;
    rb->count++;

    return rb->count;
}

int  ring_pop(ring_buf_t *rb, uint8_t *byte)
{
    if(ring_is_empty(rb)) return -1; 
    *byte = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUF_SIZE;
    rb->count--;
    return rb->count;
}
