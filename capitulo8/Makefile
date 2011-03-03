all: tcpcliente tcpserver udpcliente udpserver captura

tcpcliente: TCPCliente.c
	gcc TCPCliente.c -o TCPCliente -O2
tcpserver: TCPServer.c
	gcc TCPServer.c -o TCPServer -O2
udpcliente: UDPClient.c
	gcc UDPClient.c -o UDPClient -O2
udpserver: UDPServer.c
	gcc UDPServer.c -o UDPServer -O2
captura: captura.c
	gcc captura.c -o captura -O2

clean:
	rm -rf captura TCPServer TCPCliente UDPServer UDPClient
