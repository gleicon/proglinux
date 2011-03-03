/* 
 * client UDP 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#define PORT 	1200
int main (int argc, char **argv) {

	struct sockaddr_in remote_addr;
	struct hostent *he;
	int numbytes, sockfd;
	if (argc < 3) {
		fprintf(stderr,"Uso: %s host texto \n", argv[0]);
		exit(1);
		}
	if ((he = gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
		}

	if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) ==-1) {
		perror("socket");
		exit(1);;
		}

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port	= htons (PORT);
	remote_addr.sin_addr	= *((struct in_addr *)he->h_addr);
	
	memset(&(remote_addr.sin_zero), 0, 8);

	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, (struct sockaddr *)
		&remote_addr, sizeof (struct sockaddr))) ==-1) {
		perror("sendto");
		exit(1);
		}
	
	fprintf(stdout,"Enviados %d bytes para %s\n", numbytes, 
		inet_ntoa(remote_addr.sin_addr));
	close (sockfd);
	return 0;
	}

