#include <linux/module.h>


int init_module (void) {
        printk("<1> Hello World \n");
	return 0;
}

/* Funçao chamada quando da remoçao do mesmo */
void cleanup_module (void) {
	printk("<1> Tchau :~D\n");
}

