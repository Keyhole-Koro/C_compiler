bits 64

section .data
    fmt:    db "%d", 10, 0

extern printf

section .text
    global _start

add:
    push rbp
    mov rbp, rsp
    sub rsp, 24
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
    push rax
    pop rbx
    mov eax, dword [rbp - 12]
    add rsp, 24
    pop rbp
    ret

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 24
    mov eax, -9
    mov dword [rbp - 4], eax
    mov eax, 2
    mov dword [rbp - 8], eax
    mov eax, 4
    mov dword [rbp - 12], eax
    jmp .BRANCH.0

.BRANCH.0:
    push rax
    pop rbx
    mov eax, dword [rbp - 12]
    mov ecx, eax
    push rax
    pop rbx
    mov eax, dword [rbp - 4]
    cmp ecx, eax
    jg .BRANCH.TRUE.0
    jmp .BRANCH.FALSE.0

.BRANCH.TRUE.0:
    push rax
    pop rbx
    mov edi, dword [rbp - 4]
    mov esi, dword [rbp - 8]
    call add
    mov dword [rbp - 12], eax
    jmp .CONT.0

.BRANCH.FALSE.0:
    push rax
    pop rbx
    mov eax, dword [rbp - 4]
    mov ecx, eax
    push rax
    pop rbx
    mov eax, dword [rbp - 12]
    cmp ecx, eax
    jge .BRANCH.TRUE.1
    jmp .BRANCH.FALSE.1

.BRANCH.TRUE.1:
    mov eax, 1
    mov dword [rbp - 12], eax
    jmp .CONT.0

.BRANCH.FALSE.1:
    mov eax, 9
    mov dword [rbp - 12], eax
    jmp .CONT.0

.CONT.0:
    xor rax, rax
    mov rdi, fmt
    mov esi, dword [rbp - 12]
    call printf

    mov eax, 0
    add rsp, 24
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall


