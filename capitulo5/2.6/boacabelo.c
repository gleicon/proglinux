/* chardev.c 
 * Copyright Original (C) 1998-1999 by Ori Pomerantz
 * boacabelo.c 
 * Adaptaçao Gleicon S. Moraes 2003 p/ 2.4.x
 * Cria um char device, read only
 * Adaptacao para 2.6 Gleicon 
 */


/* Header files */

#include <linux/module.h>		/* Simbolos do mdulo */
#include <linux/fs.h>			/* As definiçes de file operations */

#include <asm/uaccess.h>		/* put_user */

/* Um detalhe importante sao as mudanças entre as versoes do kernel, 
 * da API para mdulos. Uma das mais radicais foi do 2.0.xx para 2.2.xx.
 * Portanto preve-las, e lidar com isso da a certeza de que o modulo funciona 
 * em qualquer versao, a nao ser que o desenvolvedor nao deseje isso 
 */

#define SUCCESS 0
#define DEVICE_NAME "boacabelo"	// nome do nosso device (/proc/devices)
#define BUF_LEN 80		// tamanho maximo da msg retornada

static int Device_Open = 0;		// lock para prevenir multiplos open() 
static char Message[BUF_LEN];		// buffer que contera a mensagem
static char *Message_Ptr;		// apenas um ponteiro 

/* Declaraçoes do arquivo */

/* funçao chamada quando open() é utilizado no device */
int device_open(struct inode *inode, struct file *file) { 

	static int counter = 0;

	/* Mostrando o major e o minor number do dispositivo */
	printk("<9>Device: %d.%d\n", inode->i_rdev >> 8, inode->i_rdev & 0xFF);

	/* retorna -EBUSY se outro processo estiver lendo */
	if (Device_Open) return -EBUSY; 

	/* Como nosso caso é demonstrativo e didatico, nao vamos nos preocupar 
	* com a possibilidade de concorrencia de dois processos ou dos problemas 
	* que a proxima instruçao poderia causar em um sistema SMP (com mais de 
	* um processador).
	*/
	Device_Open++; /* marca que o device esta aberto */

	/* Cuidado com buffer overflows !!!! */

	sprintf(Message, "BOA CABELO !!! Disposivo lido : %d vezes\n", counter++);

	Message_Ptr = Message; 

	/* mais um smbolo especfico dos modulos.
	* Caso o contador de uso do modulo for maior que 0, nao sera permitido
	* que ele seja removido. Aqui utilizamos este artificio como uma trava 
	* para garantir que nosso processo nao sera interrompido, o que pode causar
	* serios problemas.
	*/

	return SUCCESS;
}

/* Esta funçao fecha o device - syscall close() */

int device_release(struct inode *inode, struct file *file) {

	/* Libera o contador, pronto para a prxima. */
	Device_Open--;

	/* Liberamos o contador, para que caso queira, o modulo seja removido. */
	/* se chegou ao close(), significa que a missao foi cumprida */


	return 0;
}

/* Funço para read() */
ssize_t device_read(struct file *file, 
	char *buffer,				/* Buffer que receber os dados */
	size_t length,				/* Tamanho do buffer */
	loff_t * offset)			/* Offset dentro do arquivo */
{
 int bytes_read = 0;

 /*	Se for o fim da mensagem, retorna 0, EOF */
 if (*Message_Ptr == 0) return 0;

 /*	Coloca dados no buffer */
 while (length && *Message_Ptr) {

	/*	A magica acontece. Lembre-se que estamos em Kernel Mode, e os dados devem
		ser passados para o User Mode. Como mandar diretamente nao funcionar, 
		usamos esta funçao:
	 */
	
	put_user(*(Message_Ptr++), buffer++);

	length;
	bytes_read++;
 }

 /* Retorna o nmero de bytes lidos */
 return bytes_read;
}

/* funço para write(), mas nosso mdulo  read-only */

ssize_t device_write(struct file *file, 
	const char *buffer,			/* buffer */
	size_t length,					/* tamanho do buffer */
	loff_t * offset)				/* offset no arquivo */
{
	return -EINVAL;				/* operaçao invalida */
}

/* Declaraçoes do modulo */

/* Numero do dispositivo (Major number) */
/* Ele deve estar acessivel para as rotinas de registro e remoçao *

static int Major;

/* Sempre que trabalhamos com dispositivos, temos que usar esta estrutura
 * chamada fops (file operations, operaçoes de arquivo), para mapear as funçoes
 * corretas para cada *evento*. Como ja criamos todas as operaçes que usaremos, vamos preenche-la e colocar NULL nos campos no utilizados.
 */

							
struct file_operations Fops = {
 owner:	THIS_MODULE,
 read:	device_read,
 write:	device_write,
 open:	device_open,
 release:	device_release,		/* close */
};

/* Inicializa o modulo. Aqui aproveitamos para registrar o char device */

int init_module() {
 
 /* Tenta registrar o dispositivo */
 Major = register_chrdev(0, DEVICE_NAME, &Fops);

 /* Valor negativo significa erro */
 if (Major < 0) {
		printk("Registro do device falhou. Major: %d\n", Major);
		return Major;
 }

 /* Apenas um banner para mostrar o Major */
 
 printk("Registro do device Ok O major device number e %d.\n", Major);
 printk("Para criar o node correto e usar o device:\n");
 printk("mknod <name> c %d <minor> (normalmente 0)\n", Major);

 return 0;
}

/* Cleanup - desativa o dispositivo e limpa tudo */
void cleanup_module() {
	int ret;

	/* desativaço */
	ret = unregister_chrdev(Major, DEVICE_NAME);

	/* Reporta erro, se houver */
	if (ret < 0)
		printk("Erro ao tentar remover: %d\n", ret);
}

