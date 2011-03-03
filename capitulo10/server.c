/* TCP Socket server win32/linux */
/* 
	windows: gcc -o server server.c -l wsock32 
	linux: gcc -o server server.c 
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
#define BACKLOG 4

int main (int argc, char **argv) {

	#ifdef WIN32
	WSADATA wsda;
	#endif

	struct sockaddr_in server;
	struct sockaddr_in client;
	int sockfd, sockfd2, n_bytes;
	char msg[50];
	int size, visits = 0;
	
	#ifdef WIN32 
	WSAStartup(0x0101, &wsda);		// inicializa winsock.dll
	#endif
	
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		fprintf(stderr,"erro de socket\n");
		exit(-1);
	}

	memset(&server, 0, sizeof(struct sockaddr_in));
	
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(PORT); 
	server.sin_family = AF_INET;

	if (bind(sockfd,(struct sockaddr*)&server, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"erro bind()\n");
		exit(1);
	}
	if (listen(sockfd, BACKLOG)) { 
		fprintf(stderr, "erro listen()\n");
		exit(-1);
	}

	while (1) {
		size = sizeof(struct sockaddr_in);
		if ((sockfd2 = accept(sockfd,(struct sockaddr*)&client, &size)) == -1) {
			fprintf(stderr," erro: accept");
			exit(-1);
		}
		visits++;
		fprintf(stderr, "Conexo: [%d]" , visits);
		memset(msg, 0, sizeof(msg));
		
		if ((n_bytes = recv(sockfd2, msg, 50, 0)) == -1) {
			printf ("erro recv()");
			exit(-1);
		}
		msg[n_bytes]=\0;

		fprintf(stdout,"Mensagem:%s\n", msg);
		closesocket(sockfd2);

	}
	#ifdef WIN32
	WSACleanup();		// finaliza winsock.dll
	#endif
	closesocket(sockfd); 
	return 0;
}

