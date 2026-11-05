/*
 * bl.ld -- TP4 (Bloque 4): linker script del BOOTLOADER
 *
 * COMPLETAR los TODO (hito 2). El BL vive en BL_ROM (memory_map.ld).
 */
ENTRY(_bl_vectors)

INCLUDE memory_map.ld

SECTIONS
{
  .text : {
    /* TODO: la vector table (seccion .vectors de bl_start.S) tiene que quedar
     *       PRIMERA, en 0x0000_0000. Ojo con --gc-sections: nadie la referencia. */
    *(.vectors)
    *(.text .text.*)
    *(.rodata .rodata.* .srodata .srodata.*)
    . = ALIGN(4);
  } > BL_ROM

  /* TODO: el BL no usa variables globales. Hagan que el link FALLE si alguien
   *       agrega una (.data/.sdata o .bss/.sbss/COMMON con tamano distinto de 0).
   *       Pista: secciones con simbolos de inicio/fin + ASSERT. */

    .data : {
        *(.data)
        *(.sdata)
    } > BL_ROM

    .bss : {
        *(.bss)
        *(.sbss)
    } > BL_ROM

    ASSERT(SIZEOF(.data)<1, ".data grande")
    ASSERT(SIZEOF(.bss)<1, ".bss grande") 

    /DISCARD/ : { *(.eh_frame .eh_frame_hdr) }

    /* TODO: _bl_stack_top (lo usa bl_start.S). ¿Donde conviene que quede el stack del BL? */
   _bl_stack_top = ORIGIN(RAM) + LENGTH(RAM);

}


/* (YA ESTA HECHO) Donde espera el BL encontrar al FW: bl_main.c los usa como extern */
__fw_slot_start = ORIGIN(APP_ROM);
__fw_slot_size  = LENGTH(APP_ROM);

/* TODO: ASSERT que verifique que la vector table quedo en ORIGIN(BL_ROM). */
ASSERT(_bl_vectors == 0, "_bl_vectors direccion erronea")
