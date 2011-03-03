#include <stdio.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define FB "/dev/fb0"
int main (int argc, char **argv) {
	
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	int b,g,r; 
	unsigned short int bpp16;

	unsigned char *tela;
	int tamanho_tela;
	int fd;
	int x = 100, y = 100;
	long int coord = 0;

	
	if ((fd = open(FB, O_RDWR)) == -1) {
		perror("Erro ao abrir dispositivo");
		exit(-1);
	}

	/* Propriedades tela fixa */
	
	if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Erro: FBIOGET_FSCREENINFO");
		exit(-1);
	}

	/* Propriedades tela virtual */

	if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo)) {
		perror("Erro: FBIOGET_VSCREENINFO");
		exit(-1);
	}
	
	tamanho_tela = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	fprintf(stdout,"Dados do dispositivo: %s - %s\n", FB, finfo.id);
	fprintf(stdout,"Resoluçao:\n");
	fprintf(stdout,"Horizontal %d pixels\n", vinfo.xres);
	fprintf(stdout,"Vertical %d pixels\n", vinfo.yres);
	fprintf(stdout,"Bits por pixel %d pixels\n", vinfo.bits_per_pixel);
	fprintf(stdout,"Tamanho da tela: %d bytes\n", tamanho_tela);
	fprintf(stdout,"Extras: Visual %d - Type %d\n", finfo.visual, finfo.type);
	fprintf(stdout,"Deslocamentos e tamanho dos componentes de cor\n");	
	fprintf(stdout,"offsets: red %d, green %d, blue %d\n", vinfo.red.offset,
				vinfo.green.offset, vinfo.blue.offset);
	fprintf(stdout,"lengths: red %d, green %d, blue %d\n", vinfo.red.length,
				vinfo.green.length, vinfo.blue.length);
	fprintf(stdout,"msb: red %d, green %d, blue %d\n", vinfo.red.msb_right, 
				vinfo.green.msb_right, vinfo.blue.msb_right);
	fprintf(stdout,"nonstd %d\n", vinfo.nonstd);


	// Mapeia o dispositivo para uma regio acessivel ao usuario
	tela = mmap (0, tamanho_tela, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (tela == (unsigned char *) - 1) {
		perror("Erro: mmap");
		exit(-1);
	}

	// Desenha uma figura qualquer, acessando diretamente a memoria 
	for (y = 100; y < 300; y++) {
		for (x = 100; x <= 300; x++) {
			coord = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) 
						+ (y + vinfo.yoffset) 	* finfo.line_length;
			
			// 8 bits por pixel
			if (vinfo.bits_per_pixel == 8) {
				// escreve diretamente
				*((unsigned char*)(tela + coord)) = x % 8;
			}
			
			// 16 bits por pixel
			if (vinfo.bits_per_pixel == 16) {
				b = 10;									// Elemento de cor azul
				g = (x - 100) / 6;					// Elemento de cor verde
				r = 31 - (y - 100) / 16;		// Elemento de cor vermelha
				
				// Acomoda as cores de acordo com seu deslocamento
				bpp16 = r << vinfo.red.offset 
						| g << vinfo.green.offset 
						| b << vinfo.blue.offset;
				*((unsigned short int*)(tela + coord)) = bpp16;
			}
			
			// 24 bits por pixel
			if (vinfo.bits_per_pixel == 24) {
				b = 0xFF;
				g = (x - 100); 
				r = 0xFF - (y - 100);
				// Por ser 24bpp (3 bytes) cada byte do pixel recebe sua cor 
				// (offset 8 bits)
				*((unsigned char*)(tela + coord)) = r;
				*((unsigned char*)(tela + coord) + 1) = g;
				*((unsigned char*)(tela + coord) + 2) = b;
			}										 
		}
	}
	// Encerra o mapeamento
	munmap(0, tamanho_tela);
	// Fecha o dispositivo

	close(fd);
	exit(0);
}

