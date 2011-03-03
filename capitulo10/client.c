/* Socket TCP Client linux/windows */
/* 
	Windows: gcc -o client.exe client.c -l wsock32 
 	Linux: gcc -o client client.c 
 */

/* Include files */

#include <stdio.h>

#if defined (linux)
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define closesocket close
#endif

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#endif

#define PORT 1200

int main (int argc, char **argv) {

	#ifdef WIN32
	/* Inicializa winsock.dll */
	WSADATA wsda;
	#endif

	struct sockaddr_in server;
	int sockfd;
	struct hostent *h;
	char *message ="Alo Servidor";
	
	#ifdef WIN32
	WSAStartup(0x0101, &wsda);
	#endif

	if (argc != 2) {
		fprintf(stderr, "Uso: client [IP or SERVERNAME]\n");
		exit(-1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1) {
		fprintf(stderr,"erro de socket\n");
		exit(-1);
	}
	if ((h = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr, "erro de resoluçao de Hostname");
		exit(-1);
	}
	server.sin_addr = *((struct in_addr*)h->h_addr);
	server.sin_port = htons(PORT);
	server.sin_family = AF_INET;

	if (connect(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr))==-1) {
		fprintf(stderr,"erro de conexo\n");
		exit(-1);
	}

	send(sockfd, message, strlen(message), 0);

	#ifdef _WIN32_
	/* Finaliza winsock.dll */
	WSACleanup();
	#endif

	closesocket(sockfd);
}

