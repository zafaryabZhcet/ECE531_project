#define STDIN	0
#define STDOUT	1
#define STDERR	2

/* Same as Linux for compatibility */
/* arch/arm/include/uapi/asm/unistd.h */
#define SYSCALL_EXIT		1
//#define SYSCALL_FORK		2
#define SYSCALL_READ		3
#define SYSCALL_WRITE		4
#define SYSCALL_OPEN		5
#define SYSCALL_CLOSE		6
#define SYSCALL_WAITPID		7
#define SYSCALL_EXECVE		11
#define SYSCALL_CHDIR		12
#define SYSCALL_TIME		13
#define SYSCALL_GETPID		20
#define SYSCALL_TIMES		43
#define SYSCALL_IOCTL		54
#define SYSCALL_REBOOT		88
#define SYSCALL_STATFS		99
#define SYSCALL_STAT		106
#define SYSCALL_SYSINFO		116
#define SYSCALL_UNAME		122
#define SYSCALL_GETDENTS	141
#define SYSCALL_NANOSLEEP	162
#define SYSCALL_GETCWD		183
#define SYSCALL_VFORK		190
#define SYSCALL_CLOCK_GETTIME	263
#define SYSCALL_STATFS64		266

/* VMW syscalls */
#define SYSCALL_BLINK		8192
#define SYSCALL_SETFONT		8193
#define SYSCALL_GRADIENT	8194
#define SYSCALL_TB1		8195
/* #define SYSCALL_RUN		8196	obsolete */
/* #define SYSCALL_STOP		8197	obsolete */
#define SYSCALL_TEMPERATURE	8198
#define SYSCALL_RANDOM		8199
#define SYSCALL_FRAMEBUFFER_LOAD	8200
#define SYSCALL_MALLOC		8201	/* hack */



/********************
 * Syscalls for game
 * ******************
*/
// Added these to existing syscall numbers to use framebuffer for game in userspace
#define SYSCALL_FB_PUTPIXEL     8300
#define SYSCALL_FB_HLINE        8301
#define SYSCALL_FB_VLINE        8302
#define SYSCALL_FB_CLEAR        8303
#define SYSCALL_FB_PUSH			8304
#define SYSCALL_FB_CONSOLE_DRAW_TEXT  8305

// New syscalls for framebuffer information
#define SYSCALL_FB_GET_WIDTH    8400
#define SYSCALL_FB_GET_HEIGHT   8401
#define SYSCALL_FB_GET_DEPTH    8402

// Keyboard
#define SYSCALL_KB_ESC_PRESSED  8500
#define SYSCALL_KB_KEY_STATE    8501

//timer
#define SYSCALL_GET_CURRENT_TIME    8600

#define SYSCALL_SET_GAME_ACTIVE  8700

