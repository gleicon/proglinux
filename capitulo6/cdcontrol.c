#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/cdrom.h>

#define MAXCOM	4			/* tamanho maximo do comando */
void printhelp(); 
void cd_play(int fd); 
void cd_stop(int fd); 
void cd_pause(int fd); 
void cd_resume(int fd); 
void cd_ejec(int fd); 

int main (int argc, char **argv) {

	char comando[256];
	int fd;

	/* Checa se algum parmetro foi passado */
	if (argc <= 1) {
		fprintf(stderr, "Uso : %s /dev/cdrom ou device correto\n", argv[0]);
		exit(-1);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror(open);
		exit(1);
	}

	fprintf(stdout,"CD control - digite ? para os comandos\n");	
	/* mantem loop enquanto nao for digitado quit */
	while (1) {
		memset(comando, 0, 256);			/* limpa buffer de comandos */
		fprintf(stdout,"> ");
		fgets(comando, MAXCOM + 3, stdin);
		if (comando[0]==?) printhelp();
		else 	if (!strncmp("play", comando, MAXCOM)) cd_play(fd);
		else	if (!strncmp("stop", comando, MAXCOM)) cd_stop(fd);
		else 	if (!strncmp("paus", comando, MAXCOM)) cd_pause(fd);
		else	if (!strncmp("resu", comando, MAXCOM)) cd_resume(fd);
		else	if (!strncmp("ejec", comando, MAXCOM)) cd_ejec(fd);
		else	if (!strncmp("quit", comando, MAXCOM)) break;
		else 	fprintf(stdout,"comando invalido\n");	
	}
	fprintf(stdout,"Fechando o device\n");
	close (fd);
}

void printhelp() {
	fprintf(stdout,"CDControl \n");
	fprintf(stdout,"play - Inicia \n");
	fprintf(stdout,"stop - Para \n");
	fprintf(stdout,"paus - Pause \n");
	fprintf(stdout,"resu - Volta da pausa \n");
	fprintf(stdout,"ejec - Ejeta o CD\n");
}

void cd_play(int fd) {
	struct cdrom_tochdr toc_header;
	struct cdrom_ti track_index;
	int io;
	/* recupera informaçes do TOC do cd (table of contents) */	
	io = ioctl(fd, CDROMREADTOCHDR,(void *) &toc_header); 
	if (io == -1) {
		perror("ioctl: cdromreadtochdr");
		return;
	}
	/* preenche o indice de faixas a serem tocadas. no caso, todas */
	fprintf(stdout,"Faixa inicial: %d - Faixa final %d\n", toc_header.cdth_trk0, toc_header.cdth_trk1);
	track_index.cdti_trk0=toc_header.cdth_trk0; 
	track_index.cdti_ind0=0; 
	track_index.cdti_trk1=toc_header.cdth_trk1; 
	track_index.cdti_ind1=255; 
	/* aciona o play */
	io=ioctl(fd, CDROMPLAYTRKIND,(void *) &track_index);
	if (io==-1) {
		perror("ioctl: cdromplaytrkind");
		return;
	}
}

void cd_stop (int fd) {
	if ((ioctl(fd,CDROMSTOP,0)) == -1)
		perror("ioctl: CDROMSTOP");
}
void cd_pause (int fd) {
	if ((ioctl(fd,CDROMPAUSE,0)) == -1)
		perror("ioctl: CDROMPAUSE");
}
void cd_resume (int fd) {
	if ((ioctl(fd,CDROMRESUME,0)) == -1)
		perror("ioctl: CDROMRESUME");
}
void cd_ejec (int fd) {
	if ((ioctl(fd,CDROMEJECT,0)) == -1)
		perror("ioctl: CDROMEJECT");
}

