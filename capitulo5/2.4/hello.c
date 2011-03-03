/* Indica que estamos dentro do kernel
 * faz diferen�a na compila�ao de certos include files
 */
#define __KERNEL__
#define MODULE					/* Necessario para indicar que  � um modulo */
#include <linux/module.h>		/* Todos os simbolos e estruturas */

/* Fun�o que  chamada ao carregamento do modulo */
int init_module (void) {
	printk("<1> Hello World \n");
	return 0;
}

/* Fun�ao chamada quando da remo�ao do mesmo */

void cleanup_module (void) {
	printk("<1> Tchau :~D\n");
}

