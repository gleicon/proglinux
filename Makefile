
SUBDIRS= capitulo3 capitulo4 capitulo5

all: 
	for i in $(SUBDIRS); do \
	echo "Compilando programas em $$i"; \
	(cd $$i; make all); done
clean: 
	for i in $(SUBDIRS); do \
	echo "Compilando programas em $$i"; \
	(cd $$i; make clean); done


