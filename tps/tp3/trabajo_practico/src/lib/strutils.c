#include "strutils.h"

void strbuf_init(strbuf_t *buf)
{
    for(size_t i=0; i<sizeof(strbuf_t); i++)
        *(((char *)buf) + i) = 0; 
}

void strbuf_set(strbuf_t *buf, const char *src)
{
    int ptr = 0;

    while(src[ptr] != '\0' && ptr != STR_MAX_LEN - 1){
        buf->data[ptr] = src[ptr];
        ptr++;
    }

    buf->data[ptr] = '\0';
    buf->len = ptr;
}

size_t strbuf_append(strbuf_t *buf, const char *src)
{
    size_t ptr = buf->len; 
    size_t ptr2 = 0;
    
    while(src[ptr2] != '\0' && ptr != STR_MAX_LEN - 1){
        buf->data[ptr] = src[ptr2];
        ptr++; ptr2++;
    }
    buf->data[ptr] = 0;

    buf->len = ptr;
    return ptr2;
}

size_t strbuf_append_int(strbuf_t *buf, int32_t value)
{
    size_t ptr, ptr2;
    size_t size;

    if(buf->len == STR_MAX_LEN-1) return 0;
    
    ptr = buf->len;
    while(value && ptr < STR_MAX_LEN-1){
        buf->data[ptr] = '0' + value % 10;
        value = value / 10;
        ptr++;
    }
    size = ptr - buf->len;
    ptr2 = buf->len;
    buf->len = ptr;
    
    char tmp;
    ptr = ptr - 1;
    while(ptr > ptr2){
        tmp = buf->data[ptr2];
        buf->data[ptr2] = buf->data[ptr];
        buf->data[ptr] = tmp;
        ptr--; ptr2++;
    }

    return size;
}

size_t str_trim(char *dst, const char *src, size_t max_len)
{
    size_t ptr1 = 0;
    size_t ptr2 = 0;

    while(ptr2 != max_len-1 && src[ptr1] != 0){
        if(src[ptr1] == ' ') 
            ptr1++;
        else {
            dst[ptr2] = src[ptr1];
            ptr2++; ptr1++;
        }
    }
    dst[ptr2] = 0;
    return ptr2;
}

uint8_t str_crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = STR_CRC8_INIT;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80)
                crc = (uint8_t)((crc << 1) ^ 0x07);
            else
                crc <<= 1;
        }
    }
    return crc;
}

int    str_equal(const char *a, const char *b){
    int ptr1 = 0;
    int ptr2 = 0;
    while(a[ptr1] == b[ptr2]){
        if(a[ptr1] == 0) return 1;
        ptr1++; ptr2++;
    }
    return 0;
}
