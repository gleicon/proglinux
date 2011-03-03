[BITS 16] 		; Indica ao nasm que deve trabalhar com o conjunto de 
			; instruçoes do modo real (16 bits)
[ORG 0x7C00] 		; Endereço inicial onde a bios copiar este cdigo

start:
	mov si, bootmsg		; Carrega o endereço da mensagem
	call displaystring	; Exibe a mensagem
	
	mov cx, 02h		; 2 repetiçoes para loop
	pisca:
		mov [led], byte L
		call setleds
		mov [led], byte M
		call setleds
		mov [led], byte R
		call setleds
		loop pisca
	
	call reboot		; Simplesmente reinicia

; funçes auxiliares

; espera uma tecla
; usa int 0x16

getkey:
	mov ah, 0		; Aguarda uma tecla ser pressionada
	int 016h		; (funçao da bios)
	retn
reboot:
	mov si, resetmsg	; Carrega o endereço da mensagem
	call displaystring
	call getkey
	db 0EAh			; opcodes para forçar um reboot
	dw 0000h
	dw 0FFFFh

; funçao para imprimir uma string automaticamente 
; utilizando a int 0x10 da bios
; a string deve conter 0 no final, e ser carregada em si

displaystring:			
	lodsb
	or al,al
	jz short finish
	mov ah,0x0E	
	mov bx,0x0007
	int 0x10
	jmp displaystring
finish:
	retn

setleds:
	pusha
	mov al, 0edh			; comando Change LEDS
	out port, al			; envia para a porta 0x60
	bdelay1:
		in al, port + 4	; Le a porta de status
		test al, 02h		; testa se o registrador esta vazio
		jnz bdelay1		; continua testando se nao estiver
		mov al, [led]		; carrega o valor da variavel led
		out port, al		; manda para a porta
	bdelay2:
		in al, port + 4	; le a porta de status
		test al, 02h		; testa se esta vazio
		jnz bdelay2		; se nao, continua testando
		
		mov bx, 02h
									
	limpa:
		mov cx, 0ffffh
		loop $
		dec bx
		jne limpa
	popa
	ret

; Codigos para cada LED
R		equ		00000001b
L		equ		00000010b
M		equ		00000100b
port		equ		0x60
led		db		1

; Mensagens
bootmsg		db	'Teste de boot', 13, 10, 0
resetmsg	db	'Tecle algo para reiniciar...', 13, 10, 0

; Padding
times 510-($-$$) db 0		; Instrui o compilador a completar o binario 
				; com 00 at 509 bytes
dw 0AA55h			; Indicaçao para a BIOS de que este é um setor bootavel.
				; O proprio compilador se encarrega de armazenar
				; corretamente o valor na posiço 510 e 511
				; 512 bytes, iniciando a contagem de 0, vai at 511

