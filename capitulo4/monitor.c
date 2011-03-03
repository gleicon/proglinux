/* Monitor de CPU pela porta paralela
 * compilar com gcc monitor.c -o monitor -O2
*/
#include <stdio.h>
#include <sys/io.h>

#define MAXLEDS	8
#define LPTPORT	0x3bc
double calc_cpu_load();

int main (int argc, char **argv) {

	unsigned char lights, i;
	float cpuload;
	int a,b;
	
	if (ioperm(LPTPORT, 1, 1)) {
		fprintf(stderr, "erro ioperm.\n");
		exit(1);
	}
	i = 0;
	/* 00000000 */
	/* apaga todas as luzes */
	lights = 0;
	outb(lights, LPTPORT);
	
	/* em loop para avaliar sempre a CPU */
	while (1) {
		cpuload = calc_cpu_load();			// calcula o uso de CPU
		a = (int) ((cpuload / 100.0) * MAXLEDS);	 

		/* apaga todos para dar o efeito da retração */
		lights = 0;
		outb(lights, LPTPORT);
		
		/* acende até os leds corretos apenas */
		for (b = 0; b <= a; b++)
			lights |= (1 << b);
		outb(lights, LPTPORT);
		
		usleep(200000); /* 200 milissegundos de intervalo em cada leitura */
		i++;
	}
}

double calc_cpu_load() {
	static char buf[1024]; 
	static FILE *fp = NULL;
	static double prev_uptime = 0.0, prev_idletime = 0.0;
	char *item, *arg;

	double uptime, idletime;
	double diff_uptime, diff_idletime;
	double percentCpuLoad = -1.0;

	if ((fp = fopen("/proc/uptime", "r")) == NULL) return (percentCpuLoad);
	fscanf(fp,"%lf %lf\n", &uptime, &idletime);
	fclose(fp);

	diff_uptime = uptime - prev_uptime;
	diff_idletime = idletime - prev_idletime;
	percentCpuLoad = 100.0 - (diff_idletime / diff_uptime * 100.0);
	if (percentCpuLoad < 0.0) percentCpuLoad = 0.0;
	prev_idletime = idletime;
	prev_uptime = uptime;
	return(percentCpuLoad);
}
