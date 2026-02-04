OUTPUT_FORMAT(elf32-i386)

MEMORY{
    bootloader (rxai) : ORIGIN = 0x7c00, LENGTH = 0x1B8
    ptable (rai) : ORIGIN = 0x7dbe, LENGTH = 0x40
    signature (rai) : ORIGIN = 0x7dfe, LENGTH = 0x02
}

SECTIONS{
    bootloader : {
        *(.bootloader)
        } > bootloader
    ptable : {
        *(.ptable)
        } > ptable
    signature : {
        *(.signature)
        } > signature
}