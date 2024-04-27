bits 64
section .text
global _start
extern write, exit
_start:

    mov eax, 5
    push rax
    mov eax, 3
    push rax
    mov eax, 2
    push rax
    mov eax, 1
    pop rbx
    add eax, ebx
    pop rbx
    mov edx, 0
    idiv ebx
    push rax
    mov eax, 4
    pop rbx
    imul eax, ebx
    pop rbx
    sub eax, ebx

    mov rsi, rax
    mov rdx, 14
    mov rdi, 1
    mov rax, 1
    syscall
    ret

section .data
result_format db "Result: %d", 10, 0
