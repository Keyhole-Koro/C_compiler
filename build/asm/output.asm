bits 64

section .data
    fmt:    db "%d", 10, 0

extern printf

section .text
    global _start

_start:
    push rbp
    mov rbp, rsp
    mov eax, 3
    push rax
    mov eax, 1
    pop rbx
    sub eax, ebx
    mov dword [rbp - 4], eax
    mov eax, 2
    mov dword [rbp - 8], eax
    mov eax, 3
    push rax
    mov eax, 9
    pop rbx
    add eax, ebx
    mov dword [rbp - 4], eax
    mov rdi, fmt
    mov esi, eax
    call printf

    mov rdi, 0
    mov rax, 60
    syscall

