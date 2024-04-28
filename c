     1                                  bits 64
     2                                  
     3                                  section .data
     4 00000000 256400                  .LC0: db "%d", 0
     5                                  
     6                                  extern printf
     7                                  
     8                                  section .text
     9                                      global _start
    10                                  
    11                                  _start:
    12 00000000 B805000000                  mov eax, 5
    13 00000005 50                          push rax
    14 00000006 B803000000                  mov eax, 3
    15 0000000B 50                          push rax
    16 0000000C B802000000                  mov eax, 2
    17 00000011 50                          push rax
    18 00000012 B801000000                  mov eax, 1
    19 00000017 5B                          pop rbx
    20 00000018 01D8                        add eax, ebx
    21 0000001A 5B                          pop rbx
    22 0000001B BA00000000                  mov edx, 0
    23 00000020 F7FB                        idiv ebx
    24 00000022 50                          push rax
    25 00000023 B804000000                  mov eax, 4
    26 00000028 5B                          pop rbx
    27 00000029 0FAFC3                      imul eax, ebx
    28 0000002C 5B                          pop rbx
    29 0000002D 29D8                        sub eax, ebx
    30                                  
    31                                  
    32 0000002F 488D3D(00000000)            lea rdi, [rel .LC0]
    33 00000036 89C6                        mov esi, eax
    34 00000038 E8(00000000)                call printf
    35                                  
    36 0000003D BF00000000                  mov rdi, 0
    37 00000042 B83C000000                  mov rax, 60
    38 00000047 0F05                        syscall
