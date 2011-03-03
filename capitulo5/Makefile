
SUBDIRS= 2.4 2.6

all: 
	for i in $(SUBDIRS); do \
	echo "Compilando programas em $$i"; \
	(cd $$i; make all); done
clean: 
	for i in $(SUBDIRS); do \
	echo "Compilando programas em $$i"; \
	(cd $$i; make clean); done


