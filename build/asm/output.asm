bits 64

section .data
    fmt:    db "%d", 10, 0

extern printf

section .text
    global _start

add:
    push rbp
    mov rbp, rsp
    sub rsp, 12
    mov dword [rbp - 4], edi
    mov dword [rbp - 8], esi
    push rax
    pop rbx
    mov eax, dword [rbp - 8]
    push rax
    push rax
    pop rbx
    mov eax, dword [rbp - 4]
    pop rbx
    add eax, ebx
    mov dword [rbp - 12], eax
    xor rax, rax
    mov rdi, fmt
    mov esi, dword [rbp - 8]
    call printf

    push rax
    pop rbx
    mov eax, dword [rbp - 12]
    add rsp, 12
    pop rbp
    ret

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 12
    mov eax, -9
    mov dword [rbp - 4], eax
    mov eax, 2
    mov dword [rbp - 8], eax
    push rax
    pop rbx
    mov edi, dword [rbp - 4]
    mov esi, dword [rbp - 8]
    call add
    mov dword [rbp - 12], eax
    xor rax, rax
    mov rdi, fmt
    mov esi, dword [rbp - 8]
    call printf

    mov eax, 0
    add rsp, 12
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall


