/*
 * ps2-keyboard.c -- vmwOS driver for ps2pi PS/2 keyboard/GPIO device
 *	by Vince Weaver <vincent.weaver _at_ maine.edu>
 * Added is_esc_pressed() and few lines for game logic
 */

#include <stdint.h>
#include <stddef.h>

#include "lib/printk.h"

#include "drivers/gpio/gpio.h"
#include "drivers/keyboard/ps2-keyboard.h"
#include "drivers/console/console_io.h"

#include "interrupts/interrupts.h"

#include "time/time.h"

static int irq_num;

/* Default for the VMW ps2pi board */
int gpio_clk = 23;
int gpio_data = 24;

static unsigned keyup = 0;
static unsigned escape = 0;
static unsigned pause = 0;
volatile int esc_key_pressed = 0; // for game exiting logic

#define K_ESC 0x1b
#define K_RSV 0x80
#define K_F1 0x81
#define K_F2 0x82
#define K_F3 0x83
#define K_F4 0x84
#define K_F5 0x85
#define K_F6 0x86
#define K_F7 0x87
#define K_F8 0x88
#define K_F9 0x89
#define K_F10 0x8a
#define K_F11 0xab
#define K_F12 0x8c
#define K_ALT 0x8d
#define K_SHFT 0x8e
#define K_CTRL 0x8f
#define K_CPSL 0x90
#define K_HOME 0x91
#define K_NMLCK 0x92
#define K_SCLCK 0x93
#define K_SYSRQ 0x94
#define K_PAUSE 0x95
#define K_END 0x96
#define K_INSRT 0x97
#define K_DEL 0x98
#define K_PRINT 0x99
#define K_LEFT 0xa0
#define K_RIGHT 0xa1
#define K_UP 0xa2
#define K_DOWN 0xa3
#define K_PGDN 0xa4
#define K_PGUP 0xa5

static unsigned char translate[256] = {
	K_RSV, K_F9, K_RSV, K_F5, K_F3, K_F1, K_F2, K_F12,
	K_ESC, K_F10, K_F8, K_F6, K_F4, '\t', '`', K_RSV,
	K_RSV, K_ALT, K_SHFT, K_RSV, K_CTRL, 'q', '1', K_RSV,
	K_RSV, K_RSV, 'z', 's', 'a', 'w', '2', K_RSV,
	K_RSV, 'c', 'x', 'd', 'e', '4', '3', K_RSV,
	K_RSV, ' ', 'v', 'f', 't', 'r', '5', K_RSV,
	K_RSV, 'n', 'b', 'h', 'g', 'y', '6', K_RSV,
	K_RSV, K_ALT, 'm', 'j', 'u', '7', '8', K_RSV,
	K_RSV, ',', 'k', 'i', 'o', '0', '9', K_RSV,
	K_RSV, '.', '/', 'l', ';', 'p', '-', K_RSV,
	K_RSV, K_RSV, '\'', K_RSV, '[', '=', K_RSV, K_RSV,
	K_CPSL, K_SHFT, '\r', ']', K_RSV, '\\', K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, '\b', K_RSV,
	K_RSV, '1', K_RSV, '4', '7', K_RSV, K_HOME, K_RSV,
	'0', '.', '2', '5', '6', '8', K_ESC, K_NMLCK,
	K_F11, '+', '3', '-', '*', '9', K_SCLCK, K_RSV,
	K_RSV, K_RSV, K_RSV, K_F7, K_SYSRQ, K_RSV, K_RSV, K_RSV,
	K_PAUSE, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_ALT, K_RSV, K_RSV, K_CTRL, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, '/', K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, '\r', K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV, K_RSV,
	K_RSV, K_END, K_RSV, K_LEFT, K_HOME, K_RSV, K_RSV, K_RSV,
	K_INSRT, K_DEL, K_DOWN, K_RSV, K_RIGHT, K_UP, K_RSV, K_RSV,
	K_RSV, K_RSV, K_PGDN, K_RSV, K_PRINT, K_PGUP, K_RSV, K_RSV};

// for game including key_states
#define KEY_STATE_UP 0
#define KEY_STATE_DOWN 1
static int key_states[256] = {[0 ... 255] = KEY_STATE_UP};
static int game_active = 0;
//____

void translate_key(uint32_t key, int down)
{

	uint32_t ascii;

	static uint32_t shift_state = 0;
	static uint32_t alt_state = 0;
	static uint32_t ctrl_state = 0;

	(void)alt_state;

	ascii = translate[key];

	if (ascii == K_ALT)
	{
		if (down)
			alt_state = 1;
		else
			alt_state = 0;
		return;
	}

	if (ascii == K_CTRL)
	{
		if (down)
			ctrl_state = 1;
		else
			ctrl_state = 0;
		return;
	}

	if (ascii == K_SHFT)
	{
		if (down)
			shift_state = 1;
		else
			shift_state = 0;
		return;
	}

	// for game logic to return key_state
	//  if (down){
	//  	key_states[key] = KEY_STATE_DOWN;
	//  	return;
	//  } else{
	//  	key_states[key] = KEY_STATE_UP;
	//  }
	if (down && (ascii == 'w' || ascii == 's' || ascii == K_UP || ascii == K_DOWN))
	{
		// printk("Here\n");
		key_states[ascii] = KEY_STATE_DOWN;
	}

	/* For the time being, only report keycodes at release */
	if (down)
		return;

	if (!game_active)
	{
		if ((ascii >= 'a') && (ascii <= 'z'))
		{

			/* Convert to control chars */
			if (ctrl_state)
			{
				ascii -= 0x60;
			}

			/* convert to capital letters */
			if (shift_state)
			{
				ascii -= 0x20;
			}
		}

		console_insert_char(ascii);
	}
}

/* Handle GPIO interrupt */

int ps2_interrupt_handler(void)
{

	static unsigned key;

	int data_value;

	static int parity = 0;
	static int clock_bits = 0;
	static int message = 0;

	static uint32_t old_ticks = 0;

	/* Sanity check clock line is low? */
	//	clk_value=gpio_get_value(gpio_clk);

	if (old_ticks == 0)
	{
		old_ticks = tick_counter;
	}

	/* If it's been too long since an interrupt, clear out the char */
	/* This probably means we lost an interrupt somehow and got out */
	/* of sync.							*/
	/* We are only running at 2HZ here */
	if ((tick_counter - old_ticks) > 2)
	{
		clock_bits = 0;
		message = 0;
		parity = 0;
	}
	old_ticks = tick_counter;

	clock_bits++;

	/* read the data line */
	data_value = gpio_get_value(gpio_data);

	/* Shift in backwards as protocol is LSB first */
	parity += data_value;
	message |= (data_value << 11);
	message >>= 1;

	/* We haven't received 11 bits, so we're done for now */
	if (clock_bits != 11)
	{
		goto exit_handler;
	}

	/* We received our 11 bits */
	clock_bits = 0;

	/* Validate our 11-bit packet */
	/* FIXME: should do something useful (request resend?) if invalid */
	if (message & 0x1)
	{
		printk("Invalid start bit %x\n", message);
	}
	if (!(message & 0x400))
	{
		printk("Invaid stop bit %x\n", message);
	}
	if ((((message & 0x200 >> 8) & 0x1) + (parity & 0x1)) & 0x1)
	{
		printk("Parity error %x %x\n", message, parity);
	}

	key = (message >> 1) & 0xff;
	message = 0;
	parity = 0;

	/* Key-up events start with 0xf0 */
	if (key == 0xf0)
	{
		keyup = 1;
		goto exit_handler;
	}

	/* Extended events start with 0xe0 */
	if (key == 0xe0)
	{
		escape = 1;
		goto exit_handler;
	}

	/* Crazy pause key starts with 0xe1, has no keyup */
	if (key == 0xe1)
	{
		pause = 2;
		goto exit_handler;
	}
	if (pause == 2)
	{
		pause = 1;
		goto exit_handler;
	}
	if (pause == 1)
	{
		key = 0x88;
		pause = 0;
	}

	/* Use high bit to indicate this is an extended escape keypress */
	if (escape == 1)
	{
		key |= 0x80;
		escape = 0;
	}

	// for game exiting logic
	if (translate[key] == K_ESC)
	{
		esc_key_pressed = 1;
		// printk("Handler says esc_key_pressed: %d\n", esc_key_pressed);
	}

	/* Translate key and push to console */
	translate_key(key, !keyup);
	keyup = 0;

	// printk("Key: %x\n",key);

exit_handler:

	gpio_clear_interrupt(gpio_clk);

	return 0;
}

/* Initialize the Module */
int ps2_keyboard_init(void)
{

	uint32_t result;

	/* Allocate data/clock, use GPIO23 and GPIO24 by default */
	result = gpio_request(gpio_clk, "ps2_clock");
	if (result < 0)
		goto init_error;
	result = gpio_request(gpio_data, "ps2_data");
	if (result < 0)
		goto init_error;

	/* Set to be inputs */
	gpio_direction_input(gpio_clk);
	gpio_direction_input(gpio_data);

	/* Get interrupt number for clock input */
	irq_num = gpio_to_irq(gpio_clk);
	if (irq_num < 0)
		goto init_error;

	/* FIXME */
	/* should probe to make sure keyboard actually exists */

	/* Setup IRQ */
	gpio_set_falling(gpio_clk);
	irq_enable(irq_num);

	printk("ps2-keyboard using GPIO%d/%d, irq %d\n",
		   gpio_clk, gpio_data, irq_num);

	return 0;

init_error:

	printk("ps2-keyboard installation failed\n");

	return -1;
}

/* Remove module */
void ps2_keyboard_cleanup(void)
{

	//	free_irq(irq_num,(void *)irq_handler);

	gpio_free(gpio_data);
	gpio_free(gpio_clk);

	return;
}

// for game exiting logic
int is_esc_pressed(void)
{
	// printk("is_esc_pressed says: %d\n", esc_key_pressed);
	int state = esc_key_pressed;
	esc_key_pressed = 0;
	return state;
}

int key_state(uint32_t keyy)
{
	// printk("key = %d, %c, state = %d\n", keyy, keyy, key_states[keyy]);
	return key_states[keyy];
}

void set_game_active(int state)
{
	game_active = state;
}

// #include "drivers/keyboard/ps2-keyboard.h"
// #include "drivers/gpio/gpio.h"
// #include "lib/printk.h"
// #include <stdbool.h>
// #include <stdint.h>

// #define PS2_DATA_PIN 24
// #define PS2_CLOCK_PIN 23

// #define SCAN_CODE_EXTENDED_PREFIX 0xE0
// #define SCAN_CODE_RELEASE_PREFIX  0xF0

// static int irq_num;
// static bool isExtended = false;
// static bool isReleased = false;

// void process_scan_code(uint8_t scanCode) {
//     // Check if this is an extended scan code
//     if (scanCode == SCAN_CODE_EXTENDED_PREFIX) {
//         isExtended = true;
//         return;
//     }

//     // Check if this is a key release scan code
//     if (scanCode == SCAN_CODE_RELEASE_PREFIX) {
//         isReleased = true;
//         return;
//     }

//     // Process the scan code
//     if (isExtended) {
//         // Handle extended scan codes (like arrow keys, right Ctrl, etc.)
//         switch (scanCode) {
//             // Example: Arrow keys
//             case 0x75: // Up Arrow
//                 // Handle up arrow key
//                 break;
//             case 0x72: // Down Arrow
//                 // Handle down arrow key
//                 break;
//             // Add other extended scan codes here
//             // ...
//         }
//         isExtended = false; // Reset the extended flag
//     } else {
//         // Handle regular scan codes
//         switch (scanCode) {
//             // Example: Regular keys
//             case 0x1C: // 'A' key
//                 if (!isReleased) {
//                     // Handle 'A' key press
//                 } else {
//                     // Handle 'A' key release
//                 }
//                 break;
//             // Add other regular scan codes here
//             // ...
//         }
//         isReleased = false; // Reset the release flag
//     }
// }

// int ps2_keyboard_init() {
//     printk("Keyboard init\n");
//     int result = gpio_request(PS2_CLOCK_PIN, "ps2_clock");
//     if (result < 0) return result;

//     result = gpio_request(PS2_DATA_PIN, "ps2_data");
//     if (result < 0) return result;

//     gpio_direction_input(PS2_DATA_PIN);
//     gpio_direction_input(PS2_CLOCK_PIN);

//     gpio_set_falling(PS2_CLOCK_PIN);
//     irq_num = gpio_to_irq(PS2_CLOCK_PIN);
//     if (irq_num < 0) {
//         printk("Keyboard IRQ Error\n");
//         return -1;
//     }

//     irq_enable(irq_num);
//     printk("PS2 keyboard using GPIO%d/%d, IRQ %d\n", PS2_CLOCK_PIN, PS2_DATA_PIN, irq_num);

//     return 0;
// }

// int ps2_interrupt_handler() {
//     static unsigned char data = 0;
//     static int bit_count = 0;

//     int data_bit = gpio_get_value(PS2_DATA_PIN);

//     if (bit_count < 10) {
//         data |= (data_bit << bit_count);
//         bit_count++;
//     }

//     if (bit_count == 10) {
//         process_scan_code(data);
//         bit_count = 0;
//         data = 0;
//     }

//     gpio_clear_interrupt(PS2_CLOCK_PIN);
//     return 0;
// }

// void ps2_keyboard_cleanup() {
//     gpio_clear_interrupt(PS2_CLOCK_PIN);
//     gpio_free(PS2_DATA_PIN);
//     gpio_free(PS2_CLOCK_PIN);
// }
