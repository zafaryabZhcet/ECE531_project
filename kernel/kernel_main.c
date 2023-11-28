#include <stddef.h>
#include <stdint.h>

#include "lib/printk.h"

#include "boot/hardware_detect.h"

#include "drivers/drivers.h"
#include "drivers/block/ramdisk.h"
#include "drivers/serial/serial.h"

#include "fs/files.h"

#include "memory/memory.h"

#include "processes/idle_task.h"
#include "processes/process.h"
#include "processes/userspace.h"

#include "debug/panic.h"

/* Initrd hack */
#include "../userspace/initrd.h"


void print_boot_message(){
	char *color[] = {
		"\x1B[1;31m", // Bold Red
		"\x1B[1;33m", // Bold Yellow
		"\x1B[1;36m", // Bold Cyan
		"\x1B[1;32m", // Bold Green
		"\x1B[0m"	  // Reset
	};
	printk("\x1B[2J\x1B[H");	// Clear the screen
	printk("\x1B[1m\t\t\t\tOS is Booting...\r\n\x1B[0m");	// Print the initial booting message in bold
	// printk("\x1B[?25l"); // Hide cursor to avoid flickering during the printing
	// int i;
	// for (i = 0; i <= 3; i++)
	// {
		// if (i==3) 
		printk("\x1B[2J\x1B[H");	// Clear the screen after the third iteration
		printk("%s",color[3]);	// Set the color for the OS logo
		printk("\t\t\t\t  ___   ______ \r\n");	// Print the logo line by line
		// if (i!=3) blink_led(); 	// Blink the LED after printing each line except on the third iteration
		printk("\t\t\t\t / _ \\ / _____|\r\n");	// Print the logo line by line
		// if (i!=3) blink_led();
		printk("\t\t\t\t| | | | (_____\r\n");
		// if (i!=3) blink_led();
		printk("\t\t\t\t| | | |\\_____ \\ \r\n");
		// if (i!=3) blink_led();
		printk("\t\t\t\t| |_| | _____) |\r\n");
		// if (i!=3) blink_led();
		printk("\t\t\t\t \\___/ |______/\r\n");
		// if (i!=3) blink_led();
		printk("\x1B[6A");	// Move the cursor up by 6 lines to overwrite the logo in the next iteration
	// }
		printk("\x1B[8B");	// Move the cursor down by 8 lines

	printk("\t\t\t\t\x1B[1;32mSystem is ready!\x1B[0m\r\n"); //print message in green
	// printk("\x1B[?25h"); // Show cursor

	printk("%s",color[4]);
	printk("\x1B[2B");	// Move the cursor down by 2 lines
}
/* memory_kernel is the end of bss, so the address of the */
/* end of the kernel */
void kernel_main(uint32_t r0, uint32_t r1, uint32_t r2,
		uint32_t memory_kernel) {

	(void) r0;	/* Ignore boot method */
	uint32_t rounded_memory;

	/*******************/
	/* Detect Hardware */
	/*******************/

	hardware_detect((uint32_t *)r2);

	/* Setup Model-specific Variables based on harware revision */
	hardware_setup_vars();

	/*****************************/
	/* Initialize Serial Console */
	/*****************************/

	/* Serial console is most important so do that first */
	serial_init(SERIAL_UART_PL011);
	serial_printk("\n\n\nUsing pl011-uart\n");

	/************************/
	/* Boot messages!	*/
	/************************/

	printk("From bootloader: r0=%x r1=%x r2=%x\n",r0,r1,r2);

	/* Print boot message */
	printk("ECE531 OS\n");
	printk("Version 9\n\n");

	/* Print model info */
	hardware_print_model(r1);

	/* Print command line */
	hardware_print_commandline();

	/******************************/
	/* Enable Interrupt Hardware  */
	/******************************/

	platform_irq_enable();


	/**************************/
	/* Init Device Drivers	  */
	/**************************/

	drivers_init_all();

	/**************************/
	/* Init Memory Hierarchy  */
	/**************************/

	/* round to 1MB granularity for mem protection reasons */
	rounded_memory=memory_kernel/(1024*1024);
	rounded_memory+=1;
	rounded_memory*=(1024*1024);
	printk("Initializing memory: kernel=0x%x bytes, rounded up to 0x%x\n",
		memory_kernel,rounded_memory);
	memory_hierarchy_init(rounded_memory);

	/************************/
	/* Other init		*/
	/************************/

	/* Init the file descriptor table */
	fd_table_init();

	/* Initialize the ramdisk */
	ramdisk_init(initrd_image,sizeof(initrd_image));

	/* Mount the ramdisk */
	mount("/dev/ramdisk","/","romfs",0,NULL);

	/* Create idle task */
	create_idle_task();

	//Boot_message
	print_boot_message();

	/* Enter our "init" process */
	start_userspace("/bin/shell");

	/* we should never get here */
	printk("Error starting init!\n");

	while(1) {

		/* Loop Forever */
		/* Should probably execute a wfi instruction */
		/* In theory only here for HZ until scheduler kicks in */
	}

}
