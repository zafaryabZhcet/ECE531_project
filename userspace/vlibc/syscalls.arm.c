#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "./include/syscalls.h"

/* From Linux kernel, arch/arm/include/uapi/asm/unistd.h */
/* That is auto-generated so not always there */
/* On a real pi2 look in /usr/include/arm-linux-gnueabihf/asm/unistd.h */
#define __NR_exit	1
#define __NR_read	3
#define __NR_write	4
#define __NR_open	5
#define __NR_close	6
#define __NR_waitpid	7
#define __NR_execve	11
#define __NR_chdir	12
#define __NR_time	13
#define __NR_getpid	20
#define __NR_times	43
#define __NR_ioctl	54
#define __NR_reboot	88
#define __NR_statfs	99
#define __NR_stat	106
#define __NR_sysinfo	116
#define	__NR_uname	122
#define __NR_getdents	141
#define __NR_nanosleep  162
#define __NR_getcwd	183
#define __NR_vfork	190
#define __NR_clock_gettime	263
#define __NS_statfs64	266

int32_t exit(int32_t status) {

	register long r7 __asm__("r7") = __NR_exit;
	register long r0 __asm__("r0") = status;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0)
		: "memory");

	return r0;
}

int32_t read(int fd, void *buf, size_t count) {

	register long r7 __asm__("r7") = __NR_read;
	register long r0 __asm__("r0") = fd;
	register long r1 __asm__("r1") = (long)buf;
	register long r2 __asm__("r2") = count;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;
}


int32_t write(int fd, const void *buf, uint32_t size) {

	register long r7 __asm__("r7") = __NR_write;
	register long r0 __asm__("r0") = fd;
	register long r1 __asm__("r1") = (long)buf;
	register long r2 __asm__("r2") = size;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;
}

int32_t open(const char *filename, uint32_t flags, uint32_t mode) {

	register long r7 __asm__("r7") = __NR_open;
	register long r0 __asm__("r0") = (long)filename;
	register long r1 __asm__("r1") = flags;
	register long r2 __asm__("r2") = mode;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;
}

int32_t close(uint32_t fd) {

	register long r7 __asm__("r7") = __NR_close;
	register long r0 __asm__("r0") = fd;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0)
		: "memory");

	return r0;
}

int32_t waitpid(int32_t pid, int32_t *wstatus, int32_t options) {

	register long r7 __asm__("r7") = __NR_waitpid;
	register long r0 __asm__("r0") = (long)pid;
	register long r1 __asm__("r1") = (long)wstatus;
	register long r2 __asm__("r2") = (long)options;


	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;

}

int32_t execve(const char *filename, char *const argv[],
		char *const envp[]) {

	register long r7 __asm__("r7") = __NR_execve;
	register long r0 __asm__("r0") = (long)filename;
	register long r1 __asm__("r1") = (long)argv;
	register long r2 __asm__("r2") = (long)envp;


	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;

}

int32_t chdir(const char *path) {

	register long r7 __asm__("r7") = __NR_chdir;
	register long r0 __asm__("r0") = (long)path;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0)
		: "memory");

	return r0;
}

char *getcwd(char *buf, uint32_t size) {

	register long r7 __asm__("r7") = __NR_getcwd;
	register long r0 __asm__("r0") = (long)buf;
	register long r1 __asm__("r1") = (long)size;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1)
		: "memory");

	return (char *)r0;
}

int32_t getpid(void) {

	register long r7 __asm__("r7") = __NR_getpid;
	register long r0 __asm__("r0");

	asm volatile(
		"svc #0\n"
		: "=r"(r0) /* output */
		: "r"(r7) /* input */
		: "memory");

	return r0;

}

int32_t times(struct tms *buf) {

	register long r7 __asm__("r7") = __NR_times;
	register long r0 __asm__("r0") = (long)buf;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0)
		: "memory");

	return r0;
}

int32_t stat(const char *pathname, struct stat *buf) {

	register long r7 __asm__("r7") = __NR_stat;
	register long r0 __asm__("r0") = (long)pathname;
	register long r1 __asm__("r1") = (long)buf;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1)
		: "memory");

	return r0;
}

int32_t sysinfo(struct sysinfo *buf) {

	register long r7 __asm__("r7") = __NR_sysinfo;
	register long r0 __asm__("r0") = (long)buf;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0)
		: "memory");

	return r0;
}

int32_t uname(struct utsname *buf) {

	register long r7 __asm__("r7") = __NR_uname;
	register long r0 __asm__("r0") = (long)buf;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0)
		: "memory");

	return r0;
}

int32_t getdents(uint32_t fd, struct vmwos_dirent *dirp, uint32_t count) {

	register long r7 __asm__("r7") = __NR_getdents;
	register long r0 __asm__("r0") = (long)fd;
	register long r1 __asm__("r1") = (long)dirp;
	register long r2 __asm__("r2") = (long)count;


	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;

}

int32_t nanosleep(const struct timespec *req, struct timespec *rem) {

	register long r7 __asm__("r7") = __NR_nanosleep;
	register long r0 __asm__("r0") = (long)req;
	register long r1 __asm__("r1") = (long)rem;


	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1)
		: "memory");

	return r0;

}

int32_t vfork(void) {

	register long r7 __asm__("r7") = __NR_vfork;
	register long r0 __asm__("r0");

	asm volatile(
		"svc #0\n"
		: "=r"(r0) /* output */
		: "r"(r7) /* input */
		: "memory");

	return r0;

}

int32_t ioctl3(int fd, unsigned long request, unsigned long req2) {

	register long r7 __asm__("r7") = __NR_ioctl;
	register long r0 __asm__("r0") = fd;
	register long r1 __asm__("r1") = request;
	register long r2 __asm__("r2") = req2;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2)
		: "memory");

	return r0;
}

int32_t ioctl4(int fd, unsigned long request, unsigned long req2, unsigned long req3) {

	register long r7 __asm__("r7") = __NR_ioctl;
	register long r0 __asm__("r0") = fd;
	register long r1 __asm__("r1") = request;
	register long r2 __asm__("r2") = req2;
	register long r3 __asm__("r3") = req3;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1), "r"(r2), "r"(r3)
		: "memory");

	return r0;
}


int32_t sys_time(void) {

	register long r7 __asm__("r7") = __NR_time;
	register long r0 __asm__("r0");

	asm volatile(
		"svc #0\n"
		: "=r"(r0) /* output */
		: "r"(r7) /* input */
		: "memory");

	return r0;

}

int32_t clock_gettime(uint32_t clk_id, struct timespec *t) {

	register long r7 __asm__("r7") = __NR_clock_gettime;
	register long r0 __asm__("r0") = clk_id;
	register long r1 __asm__("r1") = (long)t;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1)
		: "memory");

	return r0;
}


int32_t sys_reboot(void) {

	register long r7 __asm__("r7") = __NR_reboot;
	register long r0 __asm__("r0");

	asm volatile(
		"svc #0\n"
		: "=r"(r0) /* output */
		: "r"(r7) /* input */
		: "memory");

	return r0;

}

int32_t fcntl(int fd, int cmd, ... /* arg */ ) {

        /* FIXME */

        return 0;
}

int statfs(const char *path, struct statfs *buf) {

	register long r7 __asm__("r7") = __NR_statfs;
	register long r0 __asm__("r0") = (long)path;
	register long r1 __asm__("r1") = (long)buf;

	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "0"(r0), "r"(r1)
		: "memory");

	return r0;
}

		/******************/
		/* GAME SPECIFIC */
		/******************/

// Code added for game
int fb_hline(int color, int x0, int x1, int y) {
	// printf("Welcome to fb_hline\n");
    register long r7 __asm__("r7") = __NR_FB_HLINE;
    register long r0 __asm__("r0") = color;
    register long r1 __asm__("r1") = x0;
    register long r2 __asm__("r2") = x1;
    register long r3 __asm__("r3") = y;
	// int result;

    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7), "r"(r0), "r"(r1), "r"(r2), "r"(r3)
        : "memory");
	
	return r0;
}

int fb_vline(int color, int y0, int y1, int x) {
	// printf("Welcome to vline\n");
    register long r7 __asm__("r7") = __NR_FB_VLINE;
    register long r0 __asm__("r0") = color;
    register long r1 __asm__("r1") = y0;
    register long r2 __asm__("r2") = y1;
    register long r3 __asm__("r3") = x;
	// int result;

    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7), "r"(r0), "r"(r1), "r"(r2), "r"(r3)
        : "memory");
	
	return r0;
}

int fb_putpixel(int color, int x, int y) {
    register long r7 __asm__("r7") = __NR_FB_PUTPIXEL;
    register long r0 __asm__("r0") = color;
    register long r1 __asm__("r1") = x;
    register long r2 __asm__("r2") = y;
	// int result;

    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7), "r"(r0), "r"(r1), "r"(r2)
        : "memory");
	
	return r0;
}

int fb_clear_screen(int color) {
	// printf("Welcome to fb_clear_screen\n");
    register long r7 __asm__("r7") = __NR_FB_CLEAR;
    register long r0 __asm__("r0") = color;
	// int result;

    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7), "r"(r0)
        : "memory");
	
	return r0;
}

int syscall_framebuffer_push(void) {
	// printf("Welcome to Usyscall_framebuffer_push\n");
    register long r7 __asm__("r7") = __NR_FB_PUSH;
	// int result;

    asm volatile(
        "svc #0\n"
        : //"=r"(result)
        : "r"(r7)
        : "memory");
	
	return 0;
}

void fbc_draw_text(int x, int y, const char *text, int color) {
    register long r7 __asm__("r7") = __NR_FB_CONSOLE_DRAW_TEXT;
    register long r0 __asm__("r0") = x;
    register long r1 __asm__("r1") = y;
    register long r2 __asm__("r2") = (long)text;
    register long r3 __asm__("r3") = color;

    asm volatile(
        "svc #0\n"
        : "=r"(r0)   // output
        : "r"(r7), "r"(r0), "r"(r1), "r"(r2), "r"(r3)   // input
        : "memory"
    );
    return r0;
}


int fb_get_width() {
	// printf("Welcome to fb_get_width\n");
    register long r7 __asm__("r7") = __NR_FB_GET_WIDTH;
    register long r0 __asm__("r0");
	
    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7)
        : "memory");

    return r0;
}

int fb_get_height() {
	// printf("Welcome to fb_get_height\n");
    register long r7 __asm__("r7") = __NR_FB_GET_HEIGHT;
    register long r0 __asm__("r0");

    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7)
        : "memory");

    return r0;
}

int fb_get_depth() {
	// printf("Welcome to fb_get_depth\n");
    register long r7 __asm__("r7") = __NR_FB_GET_DEPTH;
    register long r0 __asm__("r0");

    asm volatile(
        "svc #0\n"
        : "=r"(r0)
        : "r"(r7)
        : "memory");

    return r0;
}

int kb_esc_pressed(){
	register long r7 __asm__("r7") = __NR_KB_ESC_PRESSED;
	register long r0 __asm__("r0");
    
	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7)
		: "memory"
	);
	return r0;
}

int kb_key_state(uint32_t key){
	register long r7 __asm__("r7") = __NR_KB_KEY_STATE;
	register long r0 __asm__("r0") = key;

	asm volatile(
		"svc #0\n"
		: "=r"(r0) 	//output
		: "r"(r7), "r"(r0)	//input
		: "memory"
	) ;
	return r0;
}


int get_current_time(){
	register long r7 __asm__("r7") = __NR_GET_CURRENT_TIME;
	register long r0 __asm__("r0");
    
	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7)
		: "memory"
	);
	return r0;
}

int sys_set_game_active(int i){
	register long r7 __asm__("r7") = __NR_GAME_ACTIVE;
	register long r0 __asm__("r0") = i;
    
	asm volatile(
		"svc #0\n"
		: "=r"(r0)
		: "r"(r7), "r"(r0)
		: "memory"
	);
	return r0;
}
