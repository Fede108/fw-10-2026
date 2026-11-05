/*
 * app_startup.S — TP4 (Bloque 4): arranque del FW cuando corre detrás del BL.
 * Reemplaza a ../startup.S (que no se toca).
 *
 * COMPLETAR los TODO (hito 4).
 */
#include "fw_image.h"

#define REG_FW_STATUS_ADDR  0x00040000      /* project_config.def: REG_FW_STATUS */
#define FW_STATUS_PASS      0xFF

/* ------------------------------------------------------------------------ */
/*  1) Header + vector table del FW (app.ld lo pone al inicio de APP_ROM)    */
/* ------------------------------------------------------------------------ */
    .section .fw_header, "a"
    .balign 4
    .globl  __fw_header
    .type   __fw_header, @object
__fw_header:
    /* TODO: los 6 campos de fw_image.h, en orden (.word = 4 bytes en RISC-V).
     *   0x00 magic, 0x04 load_addr, 0x08 image_size, 0x0C image_crc,
     *   0x10 initial_sp, 0x14 reset_handler.
     *   ¿Cuáles salen de símbolos del linker? ¿Qué valor ponen en image_crc,
     *   si lo escribe tools/mkimage.py DESPUÉS de linkear? */
    .word   FW_IMAGE_MAGIC          /* 0x00 magic          (TODO) */
    .word   __fw_start              /* 0x04 load_addr      (TODO) */
    .word   __fw_size               /* 0x08 image_size     (TODO) */
    .word  FW_IMAGE_CRC_UNSET        /* 0x0C image_crc      (TODO) */
    .word   _stack_top              /* 0x10 initial_sp     (TODO) */
    .word   Reset_Handler          /* 0x14 reset_handler  (TODO) */
    .size   __fw_header, . - __fw_header

/* ------------------------------------------------------------------------ */
/*  2) Reset_Handler: "De 0 a main" (diapos 5 y 6)                           */
/* ------------------------------------------------------------------------ */
    .section .text.Reset_Handler, "ax"
    .balign 4                       /* contrato: reset_handler alineado a 4 */
    .globl  Reset_Handler
    .type   Reset_Handler, @function
Reset_Handler:
    /* TODO:
     *   1) sp = _stack_top. ¿Hace falta si el BL ya lo cargó desde la vector table?
     *   2) copiar .data: palabra por palabra de _sidata (LMA) a _sdata.._edata (VMA).
     *   3) poner .bss en cero: _sbss.._ebss.
     *   4) call main
     *   5) si main volvió y REG_FW_STATUS no quedó en FW_STATUS_PASS: ebreak.
     *      (¿qué pasa con la simulación si no lo hacen?)
     */


    la t0, _sdata
    la t1, _sidata
    la t2, _edata
    beq t0, t2, limpiar_bss
copy_data:
    lw a0, 0(t1)
    sw a0, 0(t0)
    addi t0, t0,  4
    addi t1, t1,  4
    bne t0, t2, copy_data

limpiar_bss:
    la t0, _sbss
    la t1, _ebss
    beq t0, t1, call_main
loop_bss:
    sw x0, 0(t0)
    addi t0, t0, 4
    bne t0, t1, loop_bss

call_main:
    jal ra, main
    ebreak
    unimp
    li      t0, REG_FW_STATUS_ADDR
    li      t1, FW_STATUS_PASS
    sw      t1, 0(t0)
    ebreak
    
1:  j       1b
    .size   Reset_Handler, . - Reset_Handler
