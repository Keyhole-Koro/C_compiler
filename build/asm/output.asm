bits 64
section .text
global main

main:
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

    mov [result], rax
    mov rax, result

    mov rsi, rax
    mov rdx, 8
    mov rdi, 1
    mov rax, 1
    syscall

    mov rdi, 0
    mov rax, 60
    syscall

section .data
result dq 0
