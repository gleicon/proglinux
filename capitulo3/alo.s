; Hello world em assembly
; utilizando syscalls (int 0x80)
;
; compilar com nasm -f elf alo.s
; linkar com ld -s -o alo alo.o
;
section .text align = 0
global _start 							; inicio do programa para o mundo � _start
msg db 	'Hello World', 0x0a	; msg apontar para a mensagem 
														; mais o codigo ascii para pular a linha
														; a macro db, do assembler, se traduzir
														; em uma posi�o de memria contendo a mensagem
len equ	$ - msg							; calcula o tamanho da mensagem
														; subtraindo a posi�ao de memoria atual
														; da posi�ao em que msg foi armazenada.

_start:								; ponto de execu�o inicial (entrypoint) 
	mov eax, 4					; numero da fun�o (SYS_write) write
	mov ebx, 1					; numero do fd (file descriptor) no caso,
											; saida-padrao do programa (stdout)
	mov ecx, msg				; endere�o da mensagem
	mov edx, len				; tamanho da mensagem 
											; write (1, msg, len); 
	int 0x80						; syscall
 
	mov eax, 1					; numero da fun�ao (SYS_exit) exit
	int 0x80						; chamada do kernel
;
; Fim do codigo

