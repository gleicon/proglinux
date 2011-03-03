; Exemplo 2
;
; limpa a tela
; passagem de parmetros por linha de comando
; imprime o que foi passado, muda de linha e termina
;
; os parmetros so pegos do stack
;
; compilar com : nasm -f elf prog.s
; linkar com : ld -s -o prog prog.o
;
section	.text	align = 0
global 	_start

; crlf - representa os codigos para mudar de linha 
; e retornar ao incio da mesma
line		db 0x0a
len 		equ $ - line

; limpa a tela utilizando cdigos ANSI

clrs		db 0x1b,'[2J'
clslen		equ $ - clrs

; vai para 0,0, com um bkspace 
; utilizando ANSI

home		db 0x1b,'[0H', 0x1b, '[0C', 0x08
holen		equ $ - home
; incio
_start:
; limpa a tela

	mov eax, 4			; write
	mov ebx, 1			; stdout (fd)
	mov ecx, clrs	 
	mov edx, clslen
	int 0x80			; syscall

; posiciona em 0,0
	mov eax, 4			; write
	mov ebx, 1			; stdout (fd)
	mov ecx, home	
	mov edx, holen
	int 0x80			; syscall	

; incio do processamento
	pop eax					; testa argc, para verificar se existem parmetros
	dec eax	 
	jz exit					; se nao existem, sai do programa
	pop ecx		
							; note que argv[0]  sempre o nome do programa. 
							; como nao queremos que ele apareça, faremos
							; o stackpointer andar (o indice do stack) at argv[1]

; Loop executado até que todos os argumentos sejam impressos
loop_principal:				; nome do loop principal
	
	pop ecx				; pega parmetro
	or ecx, ecx			; testa se eh zero (NULL)
	jz exit				; se for, goto (ble) exit

	mov esi, ecx
	xor edx, edx

.strlen:			; encontra o tamanho da string deste argumento (argv[])
	inc edx			; esta é uma versao simples da funçao strlen, que conta
				; cada caractere de uma string, até o seu final (\0)
	lodsb
	or al, al
	jnz .strlen
	dec edx

	mov eax , 4				; SYS_write
	mov ebx , 1				; para saida-padrao
	int 0x80				; execute a syscall

; pula linha apos escrever
	
	mov eax, 4
	mov ebx, 1
	mov ecx, line
	mov edx, len
	int 0x80

	jmp loop_principal		; continua

exit:
	mov eax, 1			; final do programa
	int 0x80			; syscall
;
;
; Fim do codigo
;

