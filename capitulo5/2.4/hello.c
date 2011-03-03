/* Indica que estamos dentro do kernel
 * faz diferença na compilaçao de certos include files
 */
#define __KERNEL__
#define MODULE					/* Necessario para indicar que  é um modulo */
#include <linux/module.h>		/* Todos os simbolos e estruturas */

/* Funço que  chamada ao carregamento do modulo */
int init_module (void) {
	printk("<1> Hello World \n");
	return 0;
}

/* Funçao chamada quando da remoçao do mesmo */

void cleanup_module (void) {
	printk("<1> Tchau :~D\n");
}

