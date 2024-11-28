#   %eax holds the system call number
#   %ebx holds the return status
#
.section .data

.section .text

.globl _start
_start:
    mov $1, %eax

    mov $10, %esi

    idiv $10, %esi

    # ebx will hold the return expression
    mov %esi, %ebx

    int $0x80
