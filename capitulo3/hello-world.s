; Exemplo 3
; Hello World utilizando libc (printf)
; compilar com nasm -felf hello-world.s
; linkar com gcc -s -o hello-world hello-world.o
;
extern printf		; declara printf como um simbolo externo
global main		; avisa que main é um ponto de entrada visivel 
			; ao mundo externo do programa.

section .text
main:
	pusha			; Salva todos os registros
	push dword msg		; Coloca o ponteiro da mensagem no stack
	call printf		; Chama a funço printf()
	add esp, 4 
	popa			; restaura seus valores
	ret			; retorna
msg: 	db 'Hello World!', 10, 0	; msg aponta para uma regiao da memoria
					; contendo nossa mensagem, seguida do caractere
					; ASCII 10 (mudança de linha) e o caractere 0
					; significando o final da string.

;
; Fim do codigo
;

