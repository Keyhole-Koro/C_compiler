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
    mov eax, 2
    push rax
    push rax
    push rax
    pop rbx
    pop rbx
    push rax
    push rax
    pop rbx
    pop rbx
    mov eax, dword [rbp - 8]
    pop rbx
    imul eax, ebx
    push rax
    push rax
    push rax
    pop rbx
    pop rbx
    push rax
    push rax
    pop rbx
    pop rbx
    mov eax, dword [rbp - 8]
    push rax
    mov eax, 9
    pop rbx
    add eax, ebx
    pop rbx
    add eax, ebx
    mov dword [rbp - 4], eax
    xor rax, rax
    mov rdi, fmt
    mov esi, [rbp-4]
    call printf

    mov rdi, 0
    mov rax, 60
    syscall

