#include <linux/module.h>
#include <linux/unistd.h>

extern void* sys_call_table[];

int (*orig_unlink)(char *path);

int new_unlink(char *buf) {
	printk("Unlink LOG: %s\n", buf);
	return orig_unlink(buf);
}

int init_module(void) {
	int i;
	
	orig_unlink = sys_call_table[__NR_unlink];
	sys_call_table[__NR_unlink]=new_unlink;
	printk("Unlink LOG: carregado\n");
	return 0;
}

void cleanup_module(void) {
	printk("Unlink LOG: removido\n");
	sys_call_table[__NR_unlink]=orig_unlink;
}

