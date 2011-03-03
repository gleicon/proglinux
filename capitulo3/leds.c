#include <stdio.h>
#include <sys/io.h>

#define 	R 1				// scroll lock led 	
#define 	L 2				// numlock led
#define 	M 4				// caps lock led
#define 	port	0x60

void setleds(unsigned char led);
int main(int argc, char **argv) {
	unsigned char led, a;
	fprintf(stdout,"Teste dos leds - preste atenço, é rapido demais.");
	ioperm(port, 5, 1);
	a = 3;
	while (a) {
		setleds(L);			// numlock
		setleds(M);			// caps
		setleds(R);			// scrool
		a = a - 1;
	}
	ioperm(port, 5, 0);
	fprintf(stdout, "..ja foi\n");
	exit(0);
}

void setleds(unsigned char led) {
	outb(0xed, port);
	while ((inb(port + 4) & 2));	// aguarda ok do controlador
	outb(led, port);
	while ((inb(port + 4) & 2));	// aguarda ok do controlador
	usleep(500);			// um tempo para nao atrapalhar o controlador
}

