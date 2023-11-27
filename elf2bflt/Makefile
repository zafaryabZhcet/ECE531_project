CC = gcc
LFLAGS =
CFLAGS = -O2 -Wall

all:	elf2bflt bflt_dump

###

bflt_dump:	bflt_dump.o
	$(CC) $(LFLAGS) -o bflt_dump bflt_dump.o

bflt_dump.o:	bflt_dump.c
	$(CC) $(CFLAGS) -c bflt_dump.c

###

elf2bflt:	elf2bflt.o
	$(CC) $(LFLAGS) -o elf2bflt elf2bflt.o

elf2bflt.o:	elf2bflt.c
	$(CC) $(CFLAGS) -c elf2bflt.c


####

clean:	
	rm -f bflt_dump elf2bflt *~ *.o
