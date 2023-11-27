char *framebuffer_init(int x, int y, int depth);
int framebuffer_clear_screen(int color);
int framebuffer_hline(int color, int x0, int x1, int y);
int framebuffer_vline(int color, int y0, int y1, int x);
int framebuffer_putpixel(int color, int x, int y);
int framebuffer_push(void);
int framebuffer_ready(void);
int framebuffer_gradient(void);
int framebuffer_load(int x, int y, int depth, char *pointer);

struct frame_buffer_info_type {
    int phys_x, phys_y;    // Physical Width / Height
    int virt_x, virt_y;    // Virtual Width / Height
    int pitch;             // Bytes per row
    int depth;             // Bits per pixel
    int x, y;              // Offset
    int pointer;           // Pointer to the framebuffer
    int size;              // Size of the framebuffer
};

// extern struct frame_buffer_info_type current_fb;
