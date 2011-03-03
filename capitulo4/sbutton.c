/********************************* Início **********************/
/*
 * Código para o teste e uso do botão serial
 * Inspirado no sled.c by Guido Socher
 * Adaptação para outros usos: Gleicon
 * 
 * 02/2003
 *
 * Decrição: 
 * sbutton -d device -e path/arquivo_exec -s estado inicial
 * -h ajuda (help)
 * -d porta serial
 * -e caminho do arquivo/script a ser executado
 * -s estado inicial do dispositivo:
 *		H, h - habilitado
 *		T, t - travado (para destravar, enviar SIGUSR2)
 *
 * Ao destravar, quando o botão for pressionado executará o parâmetro de -e
 * Exemplos:
 * 
 * sbutton -d /dev/ttyS0 -e /bin/ls -s h
 * sbutton -h
 *
 * Compilar com: gcc sbutton.c -o sbutton -O2
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>

#define OPTSTR	"hd:e:s:"

#define	DEBUG	0
//#define	DAEMON		// descomentar se for usar como daemon

#define	TEMPO	500		// tempo do loop, em microssegundos
#define	ON		1
#define	OFF		0

/* leds */

#define	VERM		0	// LED vermelho
#define	VERDE		1	// LED verde
#define	TODOS_LEDS	2	// Todos os LEDs

static int fd = 0;		/* Nosso file descriptor */
static int init_state = VERDE;	/* estado inicial VERDE */
static char *proggie;
static char *sdevice;
static char enab;

/* API do dispositivo */

int b_opendev();
void offandexit();				// saída limpa e handler dos sinais SIGINT e SIGTERM
int setled(int, int, int*);
int getpushbutton();
void inv_state();
void showhelp();
int main(int argc, char **argv) {
	int c, lstate;
	pid_t pid;
	while ((c = getopt(argc, argv, OPTSTR)) != EOF) {
		switch(c) {
			case 'e':
				proggie = malloc(strlen(optarg) + 1);
				if (!proggie) {
					perror("Malloc");
					exit(0);
				}
				strncpy(proggie, optarg, strlen(optarg));
				break;
			case 'd':
				sdevice = malloc(strlen(optarg) + 1);
				if (!sdevice) {
					perror("Malloc");
					exit(0);
				}
				strncpy (sdevice, optarg, strlen(optarg));
				break;
			case 's':
				if (*optarg != 'H' && *optarg != 'h' 
				&& *optarg != 'T' && *optarg != 't') {
					fprintf(stderr,"-s: opção inválida\n\n");
					showhelp();
				} else enab = optarg[0];
				if (enab == 't' || enab == 'T') {
					init_state = VERM;
				}
				break;
			case 'h':
			default:
				showhelp();
				break;
		}
	}
	if (DEBUG) 
		fprintf(stderr,"Porta: %s Programa: %s Status %c \n", sdevice, proggie, enab);
	b_opendev(sdevice); /* Abre a porta serial e inicializa o dispositivo */

#ifdef DAEMON
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	} else {
		exit(0);			
	}
#endif
	while (1) {				// Loop principal
		if (getpushbutton()) {
			system(proggie);
		}
		usleep(TEMPO);
	}
}
int b_opendev(char *device) {
	int lstate = 0;

	fd = open(device, O_RDWR | O_NDELAY);
	if (fd < 0) {
		fprintf(stderr, "ERRO ao abrir \"%s\"\n", device);
		exit(2);
	}
	signal(SIGINT, offandexit);
		signal(SIGTERM, offandexit);
	signal(SIGUSR2, inv_state);

	setled(init_state, ON, &lstate);		// Led no estado inicial

	return (fd);
}

/* Limpa tudo e sai */
void offandexit() {
	int state = 0;
	ioctl(fd, TIOCMSET, &state);
	close(fd);
	exit(0);
}

int setled(int whichled, int status_onoff, int *ledstate) {
	int bitpat;
	if (whichled == VERM) {
		bitpat = TIOCM_RTS;
	} else if (whichled == VERDE) {
		bitpat = TIOCM_DTR;
	} else if (whichled == TODOS_LEDS) {
		bitpat = TIOCM_RTS | TIOCM_DTR;
	}

	if (status_onoff) {
		*ledstate |= bitpat;
	} else {
		*ledstate &=~ bitpat;
	}

	if (ioctl(fd, TIOCMSET, ledstate) == -1) {
		perror("ioctl");
		exit(0);
	}
	return (0);
}

int getpushbutton() {
	int state;
	ioctl(fd, TIOCMGET, &state);			/* Lê a porta */
	if (state & TIOCM_CAR) return (1);
	else return(0);
}

void inv_state () {

	int lstate = 0;
	if ((ioctl(fd, TIOCMGET, &lstate)) == -1) {
		perror("IOCTL");
		exit(0);
	}
	setled(init_state, OFF, &lstate);		// Desliga o atual 

	init_state = !init_state;
	lstate = 0;
	setled(init_state, ON, &lstate);		// Led no estado invertido 
	if (DEBUG) fprintf(stderr, "Status invertido %d\n", init_state);
}

void showhelp() {
	fprintf(stderr,"SButton - controle via serial 0.1\n");
	fprintf(stderr,"Quando o LED VERDE estiver aceso, o botão estará habilitado.\n");
	fprintf(stderr,"O daemon pode ser executado com a opção do botão desabilitado,");
	fprintf(stderr, " o LED VERMELHO estará aceso e o processo será invertido");
	fprintf(stderr," apenas com o envio de um sinal SIGUSR2\n");
	fprintf(stderr,"Opções:\n");
	fprintf(stderr,"-d porta serial (/dev/ttyS0 por exemplo)\n");
	fprintf(stderr,"-e caminho (PATH) arquivo a ser executado \n");
	fprintf(stderr,"-s Estado inicial do dispositivo :\n");
	fprintf(stderr,"\t H ou h - Habilitado\n");
	fprintf(stderr,"\t T ou t - Travado, esperando SIGUSR2\n");
	fprintf(stderr,"-h Ajuda\n\n");
	exit(0);
}
