all: alo boot bootsect-leds hello-world leds prog leds-c

alo: alo.s
	nasm -f elf alo.s 
	ld -s -o alo alo.o

boot: boot.s
	nasm -f bin boot.s -o bootsect.bin

bootsect-leds: bootsect-leds.s 
	nasm -f bin bootsect-leds.s -o bootsect.bin

hello-world: hello-world.s
	nasm -f elf hello-world.s 
	gcc -s -o hello-world hello-world.o

leds: leds.s
	nasm -f elf alo.s 
	ld -s -o alo alo.o

prog: prog.s
	nasm -f elf prog.s 
	ld -s -o prog prog.o

leds-c: leds.c
	gcc leds.c -o leds-c

clean: 
	rm *.o *.bin leds leds-c alo prog hello-world
