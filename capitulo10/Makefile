# preencher com o seu mingw-gcc correto
GCC=mingw-gcc

all: client server teste 

client: client.c
	$(GCC) client.c -o client -lwsock32
server: server.c
	$(GCC) server.c -o server -lwsock32
teste: teste.c
	$(GCC) teste.c -o teste
clean: 
	rm teste client server teste.exe client.exe server.exe
