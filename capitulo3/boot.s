[BITS 16]		; Indica ao nasm que deve trabalhar com o conjunto de 
			; instruçoes do modo real (16 bits)
[ORG 0x7C00]		; Endereço inicial aonde a bios copiar este cdigo
start:
	mov si, bootmsg			; Carrega o endereço da mensagem de boot
	call displaystring		; exibe a mensagem
	call reboot			; simplesmente reinicia

; funçoes auxiliares
; espera uma tecla
; usa int 0x16
getkey:
	mov ah, 0			; Aguarda uma tecla ser pressionada
	int 016h			; (funçao da bios)
	retn
reboot:
	mov si, resetmsg		; Carrega o endereço da mensagem
	call displaystring
	call getkey
	db 0EAh				; Codigos do processador para forçar um reboot
	dw 0000h
	dw 0FFFFh
; funçao para imprimir uma string automaticamente utilizando a int 0x10 da bios
; a string deve conter 0 no final, e ser carregada em SI
displaystring:			
	lodsb
	or al, al
	jz short finish
	mov ah, 0x0E	
	mov bx, 0x0007
	int 0x10
	jmp displaystring
finish:
	retn
; Mensagens
bootmsg		db	'Teste de boot' , 13, 10, 0
resetmsg	db	'Tecle algo para reiniciar...' , 13, 10, 0
; Preenchimento necessario do setor de boot (Padding)
times 510-($-$$) db 0		; Instrui o compilador a completar o binario 
				; resultante com o byte 00 at completar 512 bytes
dw 0AA55h			; Indicaçao para a BIOS de que este é um setor bootavel.
				; O proprio compilador se encarrega de armazenar
				; corretamente o valor, de forma invertida (55AA)

