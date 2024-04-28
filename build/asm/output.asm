bits 64

section .data
    fmt:    db "%d", 10, 0

extern printf

section .text
    global _start

_start:
    mov eax, 9
    push rax
    mov eax, 0
    push rax
    mov eax, 9
    push rax
    mov eax, -4
    push rax
    mov eax, -2
    pop rbx
    imul eax, ebx
    push rax
    mov eax, 1
    push rax
    mov eax, 2
    push rax
    mov eax, 9
    pop rbx
    imul eax, ebx
    pop rbx
    imul eax, ebx
    pop rbx
    imul eax, ebx
    push rax
    mov eax, 5
    pop rbx
    sub eax, ebx
    pop rbx
    add eax, ebx
    pop rbx
    sub eax, ebx
    pop rbx
    sub eax, ebx

    mov rdi, fmt
    mov esi, eax
    call printf

    mov rdi, 0
    mov rax, 60
    syscall
