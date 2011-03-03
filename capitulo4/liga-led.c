/* Ferramenta para ligar os leds na porta paralela
 * liga-led <valor> , aonde valor vai de 0 a 255
 * lembre-se do valor em bin�rio antes:
 * 0 0 0 0 0 0 0 0 -> para cada posi��o um led sera aceso. A combina��o
 * das posi��es vai gerar um decimal que deve ser usado como par�metro
 *
 * compilar com gcc liga-led.c -o liga-led -O2
 *
 */
#include <stdio.h>
#include <sys/io.h>

#define MAXLEDS	8		/* n�mero de bits */
#define LPTPORT	0x3bc		/* endere�o da porta */

int main(int argc, char **argv) {

	unsigned char lights, i;
	
	if (argc < 2) {
		fprintf(stderr, "Falta o par�metro\n\n!");
		fprintf(stderr, "liga-led <n�mero>\nonde n�mero � ");
		fprintf(stderr, "o valor decimal correspondente aos leds ");
		fprintf(stderr, "que devem ser ativos, em um byte \n");
		exit(0);
	}	
	if (ioperm(LPTPORT, 1, 1)) {
		fprintf(stderr,"ioperm error.\n");
		exit(1);
	}
	i = 0;
	lights = atoi(argv[1]);;
	outb(lights, LPTPORT);
}
