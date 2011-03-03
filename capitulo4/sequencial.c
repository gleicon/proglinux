/* Seqüencial de leds pela porta paralela
 * compilar com gcc sequencial.c -o sequencial -O2
 */

#include <stdio.h>
#include <sys/io.h>

#define MAXLEDS	8
#define LPTPORT	0x3bc

int main (int argc, char **argv) {

	unsigned char lights, i;
	
	if (ioperm(LPTPORT, 1, 1)) {
		fprintf(stderr,"erro ioperm.\n");
		exit(1);
	}
	i = 0;
	lights = 0;
	outb(lights, LPTPORT);
	
	while (i < MAXLEDS) {
		lights |= (1 << i);
		outb(lights, LPTPORT);
	
		sleep(1);
		i++;
	}
	lights = 0;
	outb(lights, LPTPORT);
}
