all: boacabelo hello proc_boacabelo unlink-log

boacabelo: boacabelo.c
	gcc boacabelo.c -c -o boacabelo.o 
hello: hello.c
	gcc hello.c -c -o hello.o
proc_boacabelo: proc_boacabelo.c
	gcc proc_boacabelo.c -c -o proc_boacabelo.o
unlink-log: unlink-log.c
	gcc unlink-log.c -c -o unlink-log.o
clean:
	rm *.o
