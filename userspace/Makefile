include ../Makefile.inc

include Makefile_user.inc

GENROMFS = ../genromfs-0.5.2/genromfs

all:	shell \
	cat cat_count cause_error df hello ll ls \
	memory_test printa printb pwd slideshow sysinfo tbo uname

image:	shell \
	cat cat_count cause_error df hello ll ls \
	memory_test printa printb pwd slideshow sysinfo tbo uname
	mkdir -p image
	mkdir -p image/bin
	mkdir -p image/home
	mkdir -p image/home/ansis
	mkdir -p image/proc
	mkdir -p image/etc
	cp shell cat cat_count cause_error df \
		hello ll ls memory_test printa printb pwd slideshow \
		sysinfo tbo uname ./image/bin
	cp CATME ./image/home
	cp files/cpuinfo ./image/proc
	cp files/fstab ./image/etc
	$(GENROMFS) -f initrd.romfs -d image -V VMWos
	echo "unsigned char initrd_image[]={" > initrd.h
	xxd -i < initrd.romfs >> initrd.h
	echo "};" >> initrd.h

cat:	cat.elf
	$(ELF2BFLT) cat.elf cat

cat_count:	cat_count.elf
	$(ELF2BFLT) cat_count.elf cat_count

cause_error:	cause_error.elf
	$(ELF2BFLT) cause_error.elf cause_error

df:	df.elf
	$(ELF2BFLT) df.elf df

hello:	hello.elf
	$(ELF2BFLT) hello.elf hello

ls:	ls.elf
	$(ELF2BFLT) ls.elf ls

ll:	ll.elf
	$(ELF2BFLT) ll.elf ll

memory_test:	memory_test.elf
	$(ELF2BFLT) memory_test.elf memory_test

printa:	printa.elf
	$(ELF2BFLT) printa.elf printa

printb:	printb.elf
	$(ELF2BFLT) printb.elf printb

pwd:	pwd.elf
	$(ELF2BFLT) pwd.elf pwd

shell:	shell.elf
	$(ELF2BFLT) shell.elf shell

slideshow:	slideshow.elf
	$(ELF2BFLT) slideshow.elf slideshow

sysinfo:	sysinfo.elf
	$(ELF2BFLT) sysinfo.elf sysinfo

tbo:	tbo.elf
	$(ELF2BFLT) tbo.elf tbo

uname:	uname.elf
	$(ELF2BFLT) uname.elf uname

###


shell.elf:	shell.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o shell.elf shell.o ./vlibc/vlibc.a

cat.elf:	cat.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o cat.elf cat.o ./vlibc/vlibc.a

cat_count.elf:	cat_count.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o cat_count.elf cat_count.o ./vlibc/vlibc.a

cause_error.elf:	cause_error.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o cause_error.elf cause_error.o ./vlibc/vlibc.a

df.elf:	df.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o df.elf df.o ./vlibc/vlibc.a

hello.elf:	hello.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o hello.elf hello.o ./vlibc/vlibc.a

ls.elf:	ls.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o ls.elf ls.o ./vlibc/vlibc.a

ll.elf:	ll.o
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o ll.elf ll.o

memory_test.elf:	memory_test.o
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o memory_test.elf memory_test.o ./vlibc/vlibc.a

printa.elf:	printa.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o printa.elf printa.o ./vlibc/vlibc.a

printb.elf:	printb.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o printb.elf printb.o ./vlibc/vlibc.a

pwd.elf:	pwd.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o pwd.elf pwd.o ./vlibc/vlibc.a

slideshow.elf:	slideshow.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o slideshow.elf slideshow.o ./vlibc/vlibc.a

sysinfo.elf:	sysinfo.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o sysinfo.elf sysinfo.o ./vlibc/vlibc.a

tbo.elf:	tbo.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o tbo.elf tbo.o ./vlibc/vlibc.a

uname.elf:	uname.o ./vlibc/vlibc.a
	$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o uname.elf uname.o ./vlibc/vlibc.a

####

cat.o:	cat.c
	$(CROSS)$(CC) $(CFLAGS) -c cat.c

cat_count.o:	cat_count.c
	$(CROSS)$(CC) $(CFLAGS) -c cat_count.c

cause_error.o:	cause_error.c
	$(CROSS)$(CC) $(CFLAGS) -c cause_error.c

df.o:	df.c
	$(CROSS)$(CC) $(CFLAGS) -c df.c

hello.o:	hello.c
	$(CROSS)$(CC) $(CFLAGS) -c hello.c

memory_test.o:	memory_test.c
	$(CROSS)$(CC) $(CFLAGS) -c memory_test.c

printa.o:	printa.c
	$(CROSS)$(CC) $(CFLAGS) -c printa.c

printb.o:	printb.c
	$(CROSS)$(CC) $(CFLAGS) -c printb.c

pwd.o:	pwd.c
	$(CROSS)$(CC) $(CFLAGS) -c pwd.c

slideshow.o:	slideshow.c
	$(CROSS)$(CC) $(CFLAGS) -c slideshow.c

tbo.o:	tbo.c
	$(CROSS)$(CC) $(CFLAGS) -Wno-misleading-indentation -c tbo.c

uname.o:	uname.c
	$(CROSS)$(CC) $(CFLAGS) -c uname.c

shell.o:	shell.c
	$(CROSS)$(CC) $(CFLAGS) -c shell.c

sysinfo.o:	sysinfo.c
	$(CROSS)$(CC) $(CFLAGS) -c sysinfo.c

ls.o:	ls.c
	$(CROSS)$(CC) $(CFLAGS) -c ls.c

ll.o:	ll.arm.eabi.s logo.lzss_new
	$(CROSS)$(AS) -o ll.o ll.arm.eabi.s


./vlibc/vlibc.a:
	cd	vlibc && make

clean:
	rm -f *~ *.o *.elf \
		hello cat cat_count cause_error df ll ls memory_test \
		printa printb pwd shell slideshow sysinfo tbo uname \
		initrd.h initrd.romfs
	rm -rf image
	cd vlibc && make clean

