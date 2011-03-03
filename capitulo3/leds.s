section .text align = 0
global _start
_start:
	; Imprime uma pequena mensagem 
	mov eax, 4			; numero da funçao (syscall) write
	mov ebx,1			; numero do fd (file descriptor) - stdout
	mov ecx,msg			; endereço da mensagem
	mov edx,len			; tamanho da mensagem
	int 0x80			; executar a syscall
	
	; Preenche os registros corretamente e chama
	; a syscall ioperm para ativar a permissao
	mov eax, 101 		; numero da funçao (syscall) ioperm
	mov ebx, port 		; porta inicial
	mov ecx, 5			; numero de portas
	mov edx, 1			; status liga
	int 0x80;
	add esp, 12
	mov cx, 0ah	 

	; loop para variar o estado dos leds
	supo:
		mov [led], byte L
		call setleds
		mov [led], byte M
		call setleds
		mov [led], byte R
		call setleds
		loop supo

	; Preenche os registros corretamente e chama
	; a syscall ioperm para desativar a permisso
	mov eax, 91 			; numero da funçao (syscall) ioperm
	mov ebx, port 			; porta inicial
	mov ecx, 5				; numero de portas
	mov edx, 0				; status desliga
	int 0x80;

	; Imprime uma mensagem de sada 
	mov eax, 4			; numero da funço (syscall) write
	mov ebx,1			; numero do fd (file descriptor) - stdout
	mov ecx, msgfim			; ponteiro para a mensagem
	mov edx,lenmfim			; tamanho da mensagem
	int 0x80			; syscall
		
	; utiliza exit() para sair limpo do programa
	mov eax,1 ; sys_exit
	int 0x80			; chamada do kernel
			 
; subfunçes

setleds:
	pusha
	mov al, 0edh			; comando Change LEDS
	out port, al			; envia para a porta 0x60
	
	bdelay1:
		in al, port + 4		; Le a porta de status
		test al, 02h		; testa se o registrador esta vazio
		jnz bdelay1		; continua testando se no estiver
	mov al, [led]			; carrega o valor da varivel led
	out port, al			; manda para a porta
	bdelay2:
		in al, port + 4		; le a porta de status
		test al, 02h		; testa se esta vazio
		jnz bdelay2			; se nao, continua testando
	mov bx, 02h
	limpa:
		mov cx, 0ffffh
		loop $
		dec bx
		jne limpa
	popa
	ret

; dados do programa
section .data
R	equ	00000001b
L	equ	00000010b
M	equ	00000100b
port	equ	0x60

led		db	1
msg	db	'Teste dos leds  preste atençao, e rapido demais.', 0
len	equ	$ - msg			; calcula o tamanho da mensagem
msgfim	db	'..ja foi', 0x0d, 0x0a, 0
lenmfim	equ	$ - msgfim		; calcula o tamanho da mensagem final

