bits 16
org 0x7C00

start:
    jmp short boot_start
    nop

; FAT16 BPB
bpb:
    db "mkfs.fat"        ; OEM name (8 bytes)
    dw 512               ; bytes per sector
    db 4                 ; sectors per cluster
    dw 64                ; reserved sectors
    db 2                 ; number of FATs
    dw 512               ; root directory entries
    dw 0                 ; total sectors 16-bit
    db 0xF8               ; media descriptor
    dw 64                ; sectors per FAT
    dw 32                ; sectors per track
    dw 4                 ; number of heads
    dd 0                 ; hidden sectors
    dd 65536             ; total sectors 32-bit
    db 0x80              ; drive number
    db 0                 ; reserved
    db 0x29              ; boot signature
    dd 0x12345678        ; volume ID
    db "JANOS      "     ; volume label (11 bytes)
    db "FAT16   "        ; filesystem type (8 bytes)

boot_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; load kernel (LBA 1, 55 sectors) to physical 0x8000
    mov si, dap
    mov ah, 0x42
    mov dl, 0x80
    int 0x13
    jc disk_error

    jmp continue_boot

disk_error:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    hlt

continue_boot:
    xor ax, ax
    mov es, ax

    mov ax, 0x4F01
    mov cx, 0x0118
    mov di, 0x7E00
    int 0x10

    mov ax, 0x4F02
    mov bx, 0x0118
    or bx, 0x4000
    int 0x10

    in al, 0x92
    or al, 2
    out 0x92, al

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

; kernel loader: LBA 1, 55 sectors -> 0x0800:0x0000 = physical 0x8000
dap:
    db 0x10
    db 0
    dw 80
    dw 0x0000
    dw 0x0800
    dq 1

gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

bits 32
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000

    call 0x8000

    cli
    hlt

times 510-($-$$) db 0
dw 0xAA55
