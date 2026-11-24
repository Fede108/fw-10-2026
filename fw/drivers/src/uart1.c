/* Driver de la UART1 serie.
 *
 * Punto 3.b del TP5: completar los cuerpos marcados con TODO usando los registros de
 *         datasheets/UART1_DATASHEET.md (REG_UART1_*, definidos en config.h).
 * TP6: completar uart1_set_rts().
 *
 * Hoy los cuerpos no hacen nada (solo para que el proyecto compile): make
 * test_uart_serial tiene que fallar hasta que los implementes.
 */
#include "uart1.h"

void uart1_init(uint32_t baud_div)
{
    /* TODO hito 3.b: programar BAUD_DIV y limpiar los flags de error. */
    REG_UART1_CTRL = REG_UART1_CTRL | UART1_CTRL_CLR_ERR;       
    REG_UART1_BAUD_DIV = baud_div;
}

uint32_t uart1_rx_level(void)
{
    /* TODO hito 3.b: cantidad de bytes en el FIFO RX (ver STATUS). */
    uint32_t rx_level = UART1_ST_RX_LEVEL(REG_UART1_STATUS);
    return rx_level;
}

int uart1_getc(uint8_t *b)
{
    /* TODO 3.b: leer RX_DATA (leer saca un byte del FIFO) y devolver 1 si el
     * bit VALID estaba en 1. Si no, devolver 0 sin tocar *b. */
    
    uint16_t rx_data = REG_UART1_RX_DATA;
    if(rx_data & UART1_RX_VALID){
        *b = (uint8_t)rx_data;
        return 1;
    } else
        return 0;
}

void uart1_putc(uint8_t b)
{
    /* TODO hito 3.b: esperar a que el FIFO TX tenga lugar y escribir TX_DATA. */
    while(REG_UART1_STATUS & UART1_ST_TX_FULL);
    REG_UART1_TX_DATA = b;
}

void uart1_write(const uint8_t *buf, uint32_t n)
{
    for(uint32_t i = 0; i<n; i++){
        uart1_putc(buf[i]);
    }
}

int uart1_tx_idle(void)
{
    /* TODO hito 3.b */
    if(REG_UART1_STATUS & UART1_ST_TX_IDLE) return 1;
    return 0;
}

uint32_t uart1_errors(void)
{
    /* TODO hito 3.b: UART1_ERR_FRAME / UART1_ERR_OVERRUN segun el STATUS. */
    uint32_t errors = 0;

    if(REG_UART1_STATUS & UART1_ST_FRAME_ERR)
        errors = UART1_ERR_FRAME | errors;
    
    if(REG_UART1_STATUS &  UART1_ST_OVERRUN)
        errors = UART1_ERR_OVERRUN | errors;

    return errors; 
}

void uart1_clear_errors(void)
{
    /* TODO hito 3.b: pulsar CLR_ERR en CTRL sin cambiar el estado de RTS. */
    REG_UART1_CTRL = REG_UART1_CTRL | UART1_CTRL_CLR_ERR;       
}

/* ---- TP6 ---- */

void uart1_set_rts(int ready)
{
    /* TODO TP6 */
    (void)ready;
}
