/*
 * bl_main.c — TP4 (Bloque 4): lógica del BOOTLOADER
 *
 * El BL "hace lo mínimo": avisa que arrancó, verifica que en el slot del FW
 * haya una imagen sana y le pasa el control. No usa variables globales (todo
 * vive en el stack o es const en ROM): por eso su init de runtime es solo
 * cargar el SP (ver bl_start.S y el ASSERT de bl.ld).
 *
 * Para avisar usa lo mismo que el TP2: print_str() de platform/fw/print.c y
 * REG_FW_REPORT. BL y FW linkean cada uno su propia copia de print.c: son dos
 * binarios independientes que no comparten código.
 */
#include <stdint.h>
#include "includes.h"   /* config.h (registros) + print_str() de print.c */
#include "fw_image.h"   /* header + vector table del FW                 */
#include "strutils.h"   /* str_crc8() del TP3 (bloque3/trabajo_practico)  */

/* ---- Símbolos que define bootloader/bl.ld a partir de memory_map.ld ----
 * Para el linker son direcciones; en C se usan con '&' (o como array),
 * igual que _sdata/_ebss en un startup. */
extern const fw_image_header_t __fw_slot_start;  /* ORIGIN(APP_ROM): acá espera al FW      */
extern const uint8_t           __fw_slot_size[]; /* LENGTH(APP_ROM): el valor ES la dirección */

/* ---- bl_start.S ---- */
void bl_jump(uint32_t sp, uint32_t entry) __attribute__((noreturn));
void bl_halt(void) __attribute__((noreturn));
void bl_main(void) __attribute__((noreturn));

/* ---- Marcas de avance en REG_FW_REPORT (como test_fw_status.c del TP2):
 *      el testbench no las imprime, pero se ven en las ondas (make wave) ---- */
#define BL_REPORT_START   0xB0070000u   /* el BL arrancó              */
#define BL_REPORT_IMG_OK  0xB0070100u   /* imagen del FW validada     */
#define BL_REPORT_JUMP    0xB0070200u   /* saltando al FW             */
#define BL_REPORT_ERROR   0xB007E000u   /* error; bits bajos = código */

/* ---- REG_FW_STATUS: 0xFF termina la simulación (TP2); 0xBAD = falla, como en los tests de platform/fw ---- */
#define FW_STATUS_RUNNING  0x000u
#define FW_STATUS_FAIL     0xBADu

enum bl_error {
    BL_ERR_MAGIC = 1,   /* no hay FW en el slot (o no tiene el header del TP4)  */
    BL_ERR_LOAD_ADDR,   /* el FW fue linkeado para otra dirección               */
    BL_ERR_SIZE,        /* image_size fuera de rango                            */
    BL_ERR_CRC,         /* imagen corrupta (o no pasó por tools/mkimage.py)     */
    BL_ERR_ENTRY,       /* reset_handler fuera de la imagen o desalineado       */
    BL_ERR_SP,          /* initial_sp fuera de la RAM o no alineado a 16        */
};

/* Arma "<msg>0xXXXXXXXX" en el stack y lo manda en UNA transmisión de UART0
 * (testbench.sv imprime cada transmisión como una línea "UART: ..."). */
static void bl_log_hex(const char *msg, uint32_t val)
{
    char line[64];
    unsigned int n = 0;

    while (*msg != '\0' && n < sizeof(line) - 11u)
        line[n++] = *msg++;
    line[n++] = '0';
    line[n++] = 'x';
    for (int shift = 28; shift >= 0; shift -= 4)
        line[n++] = "0123456789ABCDEF"[(val >> shift) & 0xFu];
    line[n] = '\0';
    print_str(line);
}


static void __attribute__((noreturn)) bl_fail(enum bl_error code, const char *why)
{
    REG_FW_REPORT = BL_REPORT_ERROR | code;
    REG_FW_STATUS = 0xBAD;

    print_str(why);
    print_str("\n");
    
    /* trap: con 0xBAD solo, la simulación no termina */
    bl_halt();
}



void bl_main(void)
{
    const fw_image_header_t *hdr = &__fw_slot_start;
    const uint32_t slot_base = (uint32_t)&__fw_slot_start;
    const uint32_t slot_size = (uint32_t)__fw_slot_size;

    REG_FW_STATUS = FW_STATUS_RUNNING;
    REG_FW_REPORT = BL_REPORT_START;
    print_str("bootloader funcionando");

    if (hdr->magic != FW_IMAGE_MAGIC)
        bl_fail(BL_ERR_MAGIC, "ERR MAGIC");

    if (hdr->load_addr != (uint32_t)&__fw_slot_start)
        bl_fail(BL_ERR_LOAD_ADDR, "ERR LOAD ADDR");
    
    if ( FW_HDR_SIZE > hdr->image_size > (uint32_t)&__fw_slot_size)
        bl_fail(BL_ERR_SIZE, "ERR SIZE");

    uint8_t* start = (uint8_t*)(&__fw_slot_start);
    uint32_t crc = str_crc8(start + FW_IMAGE_CRC_START,  hdr->image_size - FW_IMAGE_CRC_START);
    if ( hdr->image_crc != crc){
        bl_log_hex("crc ", crc);
        bl_fail(BL_ERR_CRC, "ERR CRC");
    }
    bl_halt();
    
    if (hdr->initial_sp != ((ADDR_START_DATA + ADDR_SIZE_DATA)) || hdr->initial_sp & 0xF)
        bl_fail(BL_ERR_SP, "ERR SP");

    if(hdr->reset_handler > ADDR_START_DATA || hdr->reset_handler & 0x3)
        bl_fail(BL_ERR_ENTRY, "ERR ENTRY");
    
    REG_FW_REPORT = BL_REPORT_JUMP; 
    bl_jump(hdr->initial_sp, hdr->reset_handler);
    bl_fail(BL_ERR_MAGIC, "[BL] bl_main() retorna del jump"); 
}
