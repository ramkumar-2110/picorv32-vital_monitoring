ENTRY(_start)

MEMORY
{
    ROM (rwx) : ORIGIN = 0x00000000, LENGTH = 2K
}

SECTIONS
{
    . = ORIGIN(ROM);

    .text :
    {
        KEEP(*(.text.start))
        *(.text*)
    } > ROM

    .rodata :
    {
        *(.rodata*)
    } > ROM

    .data :
    {
        *(.data*)
    } > ROM

    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    } > ROM

    ASSERT(. <= ORIGIN(ROM) + LENGTH(ROM),
           "Bootloader exceeds 2 KiB ROM")
}
