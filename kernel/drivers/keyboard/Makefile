include ../../../Makefile.inc
include ../../Makefile_kernel.inc

INCLUDE = -I../../include

all:	ps2_keyboard.armv7.a

#### ARMV7

ps2_keyboard.armv7.a:	ps2_keyboard.armv7.o
	$(CROSS)$(AR) crs ps2_keyboard.armv7.a ps2_keyboard.armv7.o

ps2_keyboard.armv7.o:	ps2_keyboard.c
	$(CROSS)$(CC) $(CFLAGS_ARMV7) $(INCLUDE) -o ps2_keyboard.armv7.o -c ps2_keyboard.c

####

clean:
	rm -f *~ *.o *.a



