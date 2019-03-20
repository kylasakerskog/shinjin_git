section .data
hello_world     db      "Hello World!", 0x0a

section .text
global _start

_start:
mov rax, 4 
mov rdi, 1              ; Write to standard out = 1
mov rsi, hello_world    ; The address of hello_world string
mov rdx, 14             ; The size to write
syscall                 ; Invoke the kernel
mov rax, 1
mov rdi, 0              ; Exit success = 0
syscall                 ; Invoke the kernel