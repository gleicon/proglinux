#include <linux/module.h>


int init_module (void) {
        printk("<1> Hello World \n");
	return 0;
}

/* Fun�ao chamada quando da remo�ao do mesmo */
void cleanup_module (void) {
	printk("<1> Tchau :~D\n");
}

