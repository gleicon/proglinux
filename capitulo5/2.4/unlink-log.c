#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <sys/syscall.h>
extern void* sys_call_table[];

int (*orig_unlink)(char *path);

int new_unlink(char *buf) {
	printk("Unlink LOG: %s\n", buf);
	return orig_unlink(buf);
}

int init_module(void) {
	int i;
	EXPORT_NO_SYMBOLS;
	
	orig_unlink = sys_call_table[SYS_unlink];
	sys_call_table[SYS_unlink]=new_unlink;
	printk("Unlink LOG: carregado\n");
	return 0;
}

void cleanup_module(void) {
	printk("Unlink LOG: removido\n");
	sys_call_table[SYS_unlink]=orig_unlink;
}

