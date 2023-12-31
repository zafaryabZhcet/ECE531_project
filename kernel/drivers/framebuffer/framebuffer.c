/* based on info at https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/screen01.html */
/* http://elinux.org/RPi_Framebuffer */

/* Note if you end up with Blue and Red switched */
/* You need to update the firmware on your Pi    */

#include <stddef.h>
#include <stdint.h>

#include "lib/printk.h"

#include "drivers/firmware/mailbox.h"
// #include "drivers/framebuffer/framebuffer.h"
#include "../../include/drivers/framebuffer/framebuffer.h"

#include "lib/string.h"
#include "lib/memcpy.h"

#include "memory/mmu-common.h"

static uint32_t debug = 1;

static int framebuffer_initialized = 0;

struct frame_buffer_info_type current_fb;
static unsigned char offscreen[2048 * 2048 * 3];

// struct frame_buffer_info_type {
// 	int phys_x,phys_y;	/* IN: Physical Width / Height*/
// 	int virt_x,virt_y;	/* IN: Virtual Width / Height */
// 	int pitch;		/* OUT: bytes per row */
// 	int depth;		/* IN: bits per pixel */
// 	int x,y;		/* IN: offset to skip when copying fb */
// 	int pointer;		/* OUT: pointer to the framebuffer */
// 	int size;		/* OUT: size of the framebuffer */
// };

int framebuffer_ready(void)
{

	return framebuffer_initialized;
}

static void dump_framebuffer_info(struct frame_buffer_info_type *fb)
{

	printk("px %d py %d vx %d vy %d pitch %d depth %d x %d y %d ptr %x sz %d\n",
		   fb->phys_x, fb->phys_y,
		   fb->virt_x, fb->virt_y,
		   fb->pitch, fb->depth,
		   fb->x, fb->y,
		   fb->pointer, fb->size);

	return;
}

char *framebuffer_init(int x, int y, int depth)
{

	struct frame_buffer_info_type fb_info __attribute__((aligned(16)));
	;

	int result;
	uint32_t addr;

	fb_info.phys_x = x;
	fb_info.phys_y = y;
	fb_info.virt_x = x;
	fb_info.virt_y = y;
	fb_info.pitch = 0;
	fb_info.depth = depth;
	fb_info.x = 0;
	fb_info.y = 0;
	fb_info.pointer = 0;
	fb_info.size = 0;

	if (debug)
	{
		printk("fb: Writing message @%x to mailbox %x\n",
			   &fb_info, MAILBOX_BASE);
		printk("fb: asking for ");
		dump_framebuffer_info(&fb_info);
	}

	addr = (uint32_t)&fb_info;

	/* Flush dcache so value is in memory */
	flush_dcache((uint32_t)&fb_info, (uint32_t)&fb_info + sizeof(fb_info));

	// pi2 vs pi1
#ifdef ARMV7
	addr |= 0xC0000000;
#else
	addr |= 0x40000000;
#endif

	result = mailbox_write(addr, MAILBOX_CHAN_FRAMEBUFFER);

	if (result < 0)
	{
		printk("Mailbox write failed\n");
		return NULL;
	}

	result = mailbox_read(MAILBOX_CHAN_FRAMEBUFFER);
	if (debug)
	{
		printk("fb: we got ");
		dump_framebuffer_info(&fb_info);
	}

	if (result == -1)
	{
		printk("Mailbox read failed\n");
		return NULL;
	}

	/* Flush dcache again */
	flush_dcache((uint32_t)&fb_info, (uint32_t)&fb_info + sizeof(fb_info));

#ifdef ARMV7
	if (fb_info.pointer)
	{
		printk("FB: pointer=%x\n", fb_info.pointer);
		fb_info.pointer &= ~0xc0000000;
		printk("FB: pointer adjusted=%x\n", fb_info.pointer);
	}
#endif
	current_fb.pointer = (int)(fb_info.pointer);
	current_fb.phys_x = fb_info.phys_x;
	current_fb.phys_y = fb_info.phys_y;
	current_fb.pitch = fb_info.pitch;
	current_fb.depth = fb_info.depth;

	if (fb_info.pointer == 0)
	{
		printk("ERROR initializing framebuffer!\n");
	}
	else
	{
		framebuffer_initialized = 1;
	}

	return (char *)(fb_info.pointer);
}

int framebuffer_hline(int color, int x0, int x1, int y)
{
	// printk("framebuffer_hline: color=%d, x0=%d, x1=%d, y=%d\n", color, x0, x1, y);

	int x;
	int r, g, b;

	unsigned char *fb = offscreen;

	r = (color & 0xff0000) >> 16;
	g = (color & 0x00ff00) >> 8;
	b = color & 0x0000ff;

	for (x = x0; x < x1; x++)
	{
		fb[(y * current_fb.pitch) + (x * 3) + 0] = b;
		fb[(y * current_fb.pitch) + (x * 3) + 1] = g;
		fb[(y * current_fb.pitch) + (x * 3) + 2] = r;
	}

	return 0;
}

int framebuffer_vline(int color, int y0, int y1, int x)
{
	// printk("framebuffer_vline: color=%d, y0=%d, y1=%d, x=%d\n", color, y0, y1, x);

	int y;
	int r, g, b;

	unsigned char *fb = offscreen;

	r = (color & 0xff0000) >> 16;
	g = (color & 0x00ff00) >> 8;
	b = color & 0x0000ff;

	for (y = y0; y < y1; y++)
	{
		fb[(y * current_fb.pitch) + (x * 3) + 0] = b;
		fb[(y * current_fb.pitch) + (x * 3) + 1] = g;
		fb[(y * current_fb.pitch) + (x * 3) + 2] = r;
	}

	return 0;
}

int framebuffer_clear_screen(int color)
{
	// printk("framebuffer_clear_screen: color=%d\n", color);

	int y;

	for (y = 0; y < current_fb.phys_y; y++)
	{
		framebuffer_hline(color, 0, current_fb.phys_x, y);
	}

	return 0;
}

int framebuffer_putpixel(int color, int x, int y)
{
	// printk("framebuffer_putpixel: color=%d, x=%d, y=%d\n", color, x, y);

	int r, g, b;

	unsigned char *fb = offscreen;

	r = (color & 0xff0000) >> 16;
	g = (color & 0x00ff00) >> 8;
	b = color & 0x0000ff;

	fb[(y * current_fb.pitch) + (x * 3) + 0] = b;
	fb[(y * current_fb.pitch) + (x * 3) + 1] = g;
	fb[(y * current_fb.pitch) + (x * 3) + 2] = r;

	return 0;
}

int framebuffer_push(void)
{
	memcpy((unsigned char *)current_fb.pointer,
		   offscreen, current_fb.phys_x * current_fb.phys_y * 3);

	return 0;
}

int framebuffer_gradient(void)
{
	int y;
	uint32_t red = 50; // Starting red component

	for (y = 0; y < current_fb.phys_y; y++)
	{
		uint32_t color;

		if (y % 3 == 0)
		{
			if (y >= (current_fb.phys_y / 2))
			{
				red -= 1;
				if (red < 50)
					red = 50;
			}
			else
			{
				red += 1;
				if (red > 150)
					red = 150;
			}
		}

		color = (red << 16); // Combine the red component with the rest of the color

		framebuffer_hline(color, 0, current_fb.phys_x, y);
	}

	// Force a copy of our shadow framebuffer to the actual screen
	framebuffer_push();

	return 0;
}

int framebuffer_load(int x, int y, int depth, char *pointer)
{

	int i;

	for (i = 0; i < y; i++)
	{
		memcpy((offscreen + i * current_fb.pitch),
			   (pointer + i * 800),
			   x * (depth / 8));
	}

	/* Yes, this dual copies for now */
	framebuffer_push();

	return 0;
}
