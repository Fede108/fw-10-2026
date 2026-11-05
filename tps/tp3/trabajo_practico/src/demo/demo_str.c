#include "demo.h"
#include "print.h"
#include "strutils.h"
#include "hal_uart.h"
#include "hal_ring.h"

void buffer()
{
    ring_buf_t ring;
    ring_init(&ring);

    char* buf = "ABCDEFGHIJ";
    char buf2[10]; 

    for(int i=0; i<10; i++){
        ring_push(&ring, (uint8_t)(buf[i]));
    }

    for(int i=0; i<10; i++){
        ring_pop(&ring, (uint8_t*)(buf2 + i));
    }

    print_str("ring: ");
    print_str((const char*)buf2);
    print_str("\n");
}

void send_uart()
{
    uart_regs_t uart;
    uart_init(&uart, 115200);
    char *m = "OK";
    uart_send(&uart, (uint8_t*)m, 2);
    
    print_str("uart BRR=");
    print_dec(uart.BRR);
    print_str(" SR=0x");
    print_hex(uart.SR, 8);
    print_str(" DR=0x");
    print_hex(uart.DR, 8);
    print_str("\n");
}

void call_strutils()
{
    strbuf_t buf;
    strbuf_init(&buf);

    strbuf_set(&buf, "value=");
    strbuf_append_int(&buf, 42); 
    print_str(buf.data);
    print_str("\n");

    char dst[8]; 
    str_trim(dst, "  'hello'  ", 8);

    print_str("trimmed: ");
    print_str(dst);
    print_str("\n");

    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t crc = str_crc8(data, 4);
    
    print_str("crc8: 0x");
    print_hex(crc, 2);
    print_str("\n");
}

void demo_str(void)
{
    call_strutils();
    send_uart();
    buffer();
}
