.section .text
.global _start

_start:
    li   sp, 0x10002000   /* top of the 8 KB App RAM */
    call main
hang:
    j    hang
