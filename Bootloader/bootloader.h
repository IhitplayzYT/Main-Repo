; Bootloader.h
bits 16

struc partition
    .attrib resb 0x01
    .start resb 0x03
    .type resb 0x01
    .last resb 0x03
    .lba resd 0x01
    .nsector resd 0x01
endstruc