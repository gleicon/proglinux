#include <sys/ioctl.h>
#include <linux/kd.h>

/*
 KDSETLED - Modifica os valores dos LEDs 
 KDGETLED - L os valores dos LEDs
*/

int main(int argc, char **argv) {
	int leds = 0;
	/* loop variando os 3 bits mais baixos de leds */
	while (1) {
		leds = 2 * leds + 1;
		if (leds & 8)
			leds ^= 9;
		if (ioctl(0, KDSETLED, leds))
			exit(1);
		sleep(1);
	}
}

