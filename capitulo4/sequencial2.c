/* Seqüencial de leds pela porta paralela
 * compilar com gcc sequencial2.c -o sequencial2 -O2 
*/
#include <stdio.h>
#include <sys/io.h>

#define MAXLEDS	8
#define LPTPORT	0x3bc

int main (int argc, char **argv) {

	unsigned char lights, i;
	if (ioperm(LPTPORT,1,1)) {
		fprintf(stderr,"erro ioperm.\n");
		exit(1);
	}
	i = 0;
	lights = 0;
	outb(lights, LPTPORT);
	lights = 1;	

	while (i < MAXLEDS) { 		// Seqüência de ida		
		outb(lights, LPTPORT);
		lights = lights << 1;
	
		sleep(1);
		i++;
	}
	i = 0;
	lights = 128;		/* 10000000 */

	while (i < MAXLEDS) {		// Seqüência de volta		
		outb (lights, LPTPORT);
		lights = lights >> 1;
	
		sleep(1);
		i++;
	}
	lights = 0;
	outb(lights, LPTPORT);
}
