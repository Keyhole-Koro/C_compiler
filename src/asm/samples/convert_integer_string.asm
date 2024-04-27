section .data
    newline db 0x0A      ; Newline character
    newline_len equ $ - newline

section .bss
    num_buffer resb 20    ; Buffer to hold the string representation of the number

section .text
    global _start

_start:
    ; Number to print
    mov rax, 123456      ; Example natural number

    ; Convert the number to a string
    mov rdi, num_buffer  ; Destination buffer for the string
    call num_to_string

    ; Prepare syscall arguments
    mov rdi, 1           ; File descriptor: 1 (stdout)
    mov rsi, num_buffer  ; Pointer to the string
    mov rdx, rax         ; Length of the string (returned by num_to_string)
    mov rax, 1           ; Syscall number for sys_write

    ; Invoke syscall to print the number
    syscall

    ; Print newline
    mov rdi, 1           ; File descriptor: 1 (stdout)
    mov rsi, newline     ; Pointer to the newline character
    mov rdx, newline_len ; Length of the newline
    mov rax, 1           ; Syscall number for sys_write
    syscall

    ; Exit the program
    mov rax, 60          ; Syscall number for sys_exit
    xor rdi, rdi         ; Exit code 0
    syscall

; Function to convert a number to a string
; Input: rax - the number to convert
; Output: rdi - pointer to the buffer containing the string representation
;         rax - length of the string
num_to_string:
    push rbx
    mov rbx, 10          ; Base 10
    mov rsi, rdi         ; Store destination buffer address
    mov rdi, num_buffer  ; Destination buffer
    mov rcx, 0           ; Counter for number of digits

.convert_loop:
    xor rdx, rdx         ; Clear remainder
    div rbx              ; Divide rax by 10, quotient in rax, remainder in rdx
    add dl, '0'          ; Convert remainder to ASCII
    mov [rdi], dl        ; Store ASCII character in buffer
    inc rdi              ; Move buffer pointer
    inc rcx              ; Increment digit count
    test rax, rax        ; Check if quotient is zero
    jnz .convert_loop    ; If not zero, continue loop
.reverse_string:
    mov rsi, rdi         ; Point to end of string
    dec rsi              ; Back up one character (null terminator)
.reverse_loop:
    mov al, [rdi]        ; Load character from end of string
    mov ah, [rsi]        ; Load character from start of string
    mov [rdi], ah        ; Swap characters
    mov [rsi], al
    inc rdi              ; Move towards start of string
    dec rsi              ; Move towards end of string
    cmp rdi, rsi         ; Check if pointers have crossed
    jge .done            ; If yes, string is reversed
    jmp .reverse_loop
.done:
    mov rax, rcx         ; Length of the string
    pop rbx
    ret
