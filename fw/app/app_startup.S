/*
 * app_startup.S — TP4 (Bloque 4): arranque del FW cuando corre detrás del BL.
 * Reemplaza a platform/fw/startup.S, que solo hacía "la sp, _stack_top; call main".
 *
 *  1) Header + vector table del FW (sección .fw_header, primera en APP_ROM):
 *     el BL la lee para validar la imagen y saber a dónde saltar.
 *  2) Reset_Handler: el "De 0 a main" completo (diapos 5 y 6 del Bloque 4).
 */
#include "fw_image.h"

#define REG_FW_STATUS_ADDR  0x00040000      /* project_config.def: REG_FW_STATUS */
#define FW_STATUS_PASS      0xFF

/* ------------------------------------------------------------------------ */
/*  1) Header + vector table (app.ld la pone en 0x0000_1000)                 */
/* ------------------------------------------------------------------------ */
    .section .fw_header, "a"
    .balign 4
    .globl  __fw_header
    .type   __fw_header, @object
__fw_header:
    .word   FW_IMAGE_MAGIC          /* 0x00 magic "TP4F"                               */
    .word   __fw_start              /* 0x04 load_addr  (linker: dónde quedó linkeado)  */
    .word   __fw_size               /* 0x08 image_size (linker: app.ld)                */
    .word   FW_IMAGE_CRC_UNSET      /* 0x0C image_crc  (lo escribe tools/mkimage.py)   */
    /* --- vector table del FW: direcciones, como en Cortex-M --- */
    .word   _stack_top              /* 0x10 initial_sp                                 */
    .word   Reset_Handler           /* 0x14 reset_handler                              */

    .size   __fw_header, . - __fw_header

/* ------------------------------------------------------------------------ */
/*  2) Reset_Handler: "De 0 a main"                                          */
/* ------------------------------------------------------------------------ */
    .section .text.Reset_Handler, "ax"
    .balign 4                       /* contrato: reset_handler alineado a 4, aun con -march=..c */
    .globl  Reset_Handler
    .type   Reset_Handler, @function
Reset_Handler:
    la      sp, _stack_top
     
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
    j main
    li      t0, REG_FW_STATUS_ADDR
    li      t1, FW_STATUS_PASS
    sw      t1, 0(t0)
    ebreak

1:  j       1b
    .size   Reset_Handler, . - Reset_Handler
