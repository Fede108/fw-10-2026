#pragma once

#include "includes.h"    
#include <stdint.h>
#include <stddef.h>

void uart_init();
void uart_write_byte(char c);
size_t uart_write_str(const char *s);

