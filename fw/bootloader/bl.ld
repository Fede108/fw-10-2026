/*
 * bl.ld -- TP4 (Bloque 4): linker script del BOOTLOADER
 *
 * - El BL vive en BL_ROM, desde 0x0000_0000 (PROGADDR_RESET de picorv32).
 * - Lo primero es su vector table (seccion .vectors de bl_start.S).
 * - No tiene .data ni .bss: su init de runtime es solo cargar el SP.
 * - Exporta a C donde tiene que buscar al FW (__fw_slot_start/__fw_slot_size).
 */
ENTRY(_bl_vectors)

INCLUDE memory_map.ld

SECTIONS
{
  .text : {
    KEEP(*(.vectors))                          /* vector table del BL: en 0x0 */
    *(.text .text.*)
    *(.rodata .rodata.* .srodata .srodata.*)
    . = ALIGN(4);
  } > BL_ROM

  /* El BL no usa variables globales. Si aparecen, el link falla (ASSERT abajo)
   * en lugar de generar un BL que lee basura de la RAM. */
  .data : {
    __bl_data_start = .;
    *(.data .data.* .sdata .sdata.*)
    __bl_data_end = .;
  } > RAM AT > BL_ROM

  .bss (NOLOAD) : {
    __bl_bss_start = .;
    *(.bss .bss.* .sbss .sbss.* COMMON)
    __bl_bss_end = .;
  } > RAM

    ASSERT(SIZEOF(.data)<1, ".data grande")
    ASSERT(SIZEOF(.bss)<1, ".bss grande") 

    /DISCARD/ : { *(.eh_frame .eh_frame_hdr) }

    /* TODO: _bl_stack_top (lo usa bl_start.S). ¿Donde conviene que quede el stack del BL? */
   _bl_stack_top = ORIGIN(RAM) + LENGTH(RAM);

}

* Donde espera el BL encontrar al FW (los usa bl_main.c como extern) */
__fw_slot_start = ORIGIN(APP_ROM);
__fw_slot_size  = LENGTH(APP_ROM);

ASSERT(_bl_vectors == ORIGIN(BL_ROM), "_bl_vectors direccion erronea")
