section .data
hello_world     db      "Hello World!", 0x0a

section .text
global _start

_start:
mov rax, 1 		; write
mov rdi, 1              ; stdout
mov rsi, hello_world    ; The address of hello_world string
mov rdx, 14             ; The size to write
syscall                 
mov rax, 60		; exit
mov rdi, 0              ; Exit success = 0, stdin
syscall                 
