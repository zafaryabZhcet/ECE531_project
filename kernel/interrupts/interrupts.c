#include <stddef.h>
#include <stdint.h>

#include "drivers/bcm2835/bcm2835_io.h"
#include "drivers/bcm2835/bcm2835_periph.h"
#include "drivers/keyboard/ps2-keyboard.h"
#include "drivers/timer/timer.h"
#include "drivers/serial/serial.h"

#include "lib/printk.h"
#include "lib/mmio.h"

#include "time/time.h"
#include "processes/scheduler.h"
#include "processes/exit.h"

#include "boot/hardware_detect.h"
#include "interrupts/gic-400.h"


#define MAX_IRQ	64

int platform_irq_enable(void) {

	/* Enable Pi4 gic400 Interrupt Controller */

	if (hardware_type==RPI_MODEL_4B) {
		printk("Enabling GIC-400 on Pi4B\n");
		gic400_init((void *)0xFF840000UL);
	}

	return 0;

}

int irq_enable(int which_one) {

	uint32_t address_offset,bit,old;

        if (which_one>MAX_IRQ) {
                printk("IRQ%d too big\n",which_one);
                return -1;
        }

        bit=1<<(which_one&0x1f);
        address_offset=(which_one/32)*4;

        old=bcm2835_read(IRQ_ENABLE_IRQ1+address_offset);
        bcm2835_write(IRQ_ENABLE_IRQ1+address_offset,old|bit);

        return 0;
}


int irq_disable(int which_one) {

	uint32_t address_offset,bit,old;

        if (which_one>MAX_IRQ) {
                printk("IRQ%d too big\n",which_one);
                return -1;
        }

        bit=1<<(which_one&0x1f);
        address_offset=(which_one/32)*4;

        old=bcm2835_read(IRQ_DISABLE_IRQ1+address_offset);
        bcm2835_write(IRQ_DISABLE_IRQ1+address_offset,old|bit);

        return 0;

}

#if 0

static void user_reg_dump(void) {

	unsigned long regs[15];
	unsigned long saved_sp;
	int i;

	asm volatile(
		"str sp,%[saved_sp]\n"
                "mov sp,%[regs]\n"
                "stmia sp, {r0 - lr}^\n"        /* the ^ means load user regs */
		"ldr sp,%[saved_sp]\n"
                : [saved_sp]"=m"(saved_sp)/* output */
                :       [regs] "r"(regs) /* input */
                : "sp", "memory" /* clobbers */
                        );

	for(i=0;i<15;i++) {
		printk("reg%d = %x ",i,regs[i]);
	}
	printk("\n");
}

#endif

void interrupt_handler_c(void) {

	uint32_t basic_pending,pending0,pending1,pending2;
	uint32_t handled=0;

	/* Ideally we'd have a proper irq registration mechanism for this */
	if (hardware_type==RPI_MODEL_4B) {

		/* Note, based on IRQ_PENDING2 you can tell */
		/* 	if there's an interrupt in PENDING0/PENDING1 */
		pending0=bcm2835_read(IRQ0_PENDING0);
		pending1=bcm2835_read(IRQ0_PENDING1);
		pending2=bcm2835_read(IRQ0_PENDING2);

		// Check if UART (irq57)
		if (pending1 & IRQ_PENDING1_IRQ57) {
			handled++;
			serial_interrupt_handler();
		}

		// check if it's a timer interrupt
		if (pending2 & IRQ_PENDING2_TIMER_IRQ) {
			timer_interrupt_handler();
			handled++;
		}

		if (!handled) {
			printk("Unknown interrupt happened %x %x %x!\n",
					pending0,pending1,pending2);
			return;
		}

	}

	else {

		// Check if UART (irq57)
		basic_pending=bcm2835_read(IRQ_BASIC_PENDING);

		if (basic_pending & IRQ_BASIC_PENDING_IRQ57) {
			handled++;
			serial_interrupt_handler();
		}

		// check if it's a timer interrupt
		if (basic_pending & IRQ_BASIC_PENDING_TIMER) {
			timer_interrupt_handler();
			handled++;
		}

		// check if Keyboard interrupt
		if (basic_pending & 0X200){
			ps2_interrupt_handler();
			handled++;
		}

		if (!handled) {
			// printk("Unknown interrupt happened %x!\n",basic_pending);
			return;
		}

	}


	if (hardware_type == RPI_MODEL_4B) {
		mmio_write(GICD_ICPENDR0,0xffffffffUL);        // clear pending
	}

	if (scheduling_enabled) schedule();

	return;
}

void __attribute__((interrupt("FIQ"))) fiq_handler(void) {
	printk("UNHANDLED FIQ\n");
}

/* 1415 */
void __attribute__((interrupt("ABORT"))) data_abort_handler(void) {
	uint32_t dfsr,dfar,fs;
	register long lr asm ("lr");

	printk("MEMORY ABORT at PC=%x\n",lr-8);

	/* Read DFSR reg (see B4.1.52) */
	asm volatile("mrc p15, 0, %0, c5, c0, 0" : "=r" (dfsr) : : "cc");

	/* Read DFAR reg (see B4.1.52) */
	asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r" (dfar) : : "cc");

	fs=dfsr&0xf;

	if (fs==1) printk("\tAlignment fault\n");
	if (fs==2) printk("\tDebug event\n");
	if ((fs&0xd)==0xd) printk("\tPermission fault accessing %x\n",dfar);

	exit(-1);

}

/* 1637 */
void __attribute__((interrupt("ABORT"))) prefetch_abort_handler(void) {
	uint32_t ifsr,ifar,fs;
	register long lr asm ("lr");

	printk("PREFETCH ABORT at PC=%x\n",lr-4);

	/* Read IFSR reg (see B4.1.96) */
	asm volatile("mrc p15, 0, %0, c5, c0, 1" : "=r" (ifsr) : : "cc");

	/* Read IFAR reg (see B4.1.96) */
	asm volatile("mrc p15, 0, %0, c6, c0, 2" : "=r" (ifar) : : "cc");

	fs=ifsr&0xf;

	if (fs==2) printk("\tDebug event\n");
	if ((fs&0xd)==0xd) printk("\tPermission fault accessing %x\n",ifar);

	exit(-1);

}


void __attribute__((interrupt("UNDEF"))) undef_handler(void) {

	register long lr asm ("lr");

	long *ptr=(long *)lr;

	/* point back 4 bytes */
	ptr--;

	printk("UNDEFINED INSTRUCTION, PC=%x, insn=%x\n",lr-4,*ptr);

}
