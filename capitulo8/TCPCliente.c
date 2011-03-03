#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 1200

int main (int argc, char **argv) {
	struct sockaddr_in server;
	int sockfd;
	struct hostent *h;
	char *message ="Alo Servidor";
	if (argc != 2) {
		fprintf(stderr,"Uso: client [IP ]\n");
		exit(-1);
		}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr,"Erro de Socket\n");
		exit(-1);
		}
	if ((h = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr,"Erro de resoluçao de Hostname");
		exit(-1);
		}
	server.sin_addr =*((struct in_addr*)h->h_addr);
	server.sin_port = htons(PORT);
	server.sin_family = AF_INET;
	if (connect(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"Erro na conexo\n");
		exit(-1);
		}
	send(sockfd, message, strlen(message), 0);
	close(sockfd);
}

