#include "demo.h"
#include "print.h"
#include "strutils.h"
#include "hal_timer.h"
#include "hal_ring.h"

void demo_embedded(void)
{
    ring_buf_t ring;
    ring_init(&ring);

    uint8_t src[6] = {0xAA, 0x01, 0x02, 0x03, 0x04, 0x00};
    uint8_t dst[6];
    uint8_t crc = str_crc8(src+1, 4);
    src[5] = crc;

    for(int i=0; i<6; i++){
        ring_push(&ring, src[i]);
    }

    for(int i=0; i<6; i++){
        ring_pop(&ring, dst + i);
    }
 
    crc = str_crc8(dst + 1, 4);
    
    if (crc == dst[5] && dst[0] == 0xAA) {
        print_str("frame: ok crc=");
        print_hex(crc,2);
        print_str("\n");
    }
    else {
        print_str("frame: error\n");
    }

    timer_init();
    timer_tick();
    timer_tick();
    timer_tick();

    print_str("timer_ms: ");
    print_dec(timer_get_ms());
    print_str("\n");
}
