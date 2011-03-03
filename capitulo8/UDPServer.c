/* 
 * server UDP
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h>
#define PORT	1200
#define BUFLEN	512		// buffer
int main () {
	int sockfd;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	int addr_len, numbytes;
	char buf [BUFLEN];
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
		}
	local_addr.sin_family 	= AF_INET;
	local_addr.sin_port	= htons(PORT);
	local_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(local_addr.sin_zero), 0, 8);
	
	if (bind(sockfd, (struct sockaddr *) &local_addr, sizeof (struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}
	addr_len = sizeof (struct sockaddr);
	while (1) {
		numbytes = recvfrom (sockfd, buf, BUFLEN, 0, (struct sockaddr *) 
			&remote_addr, &addr_len);
		if (numbytes == -1) {
			perror("recvfrom");
			exit(1);
		} else {
			buf[numbytes] = 0;
			fprintf(stdout,"Pacote de %s - %d bytes Conteudo: %s\n", 
					inet_ntoa(remote_addr.sin_addr), numbytes, buf);
		}
	}
	close (sockfd);
}

