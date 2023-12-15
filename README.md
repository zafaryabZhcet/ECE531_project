# ECE531 Project

## Files that I Edited or Added

- `userspace/vlibc/game.c`: Contains the logic of pong game
- `userspace/vlib/include/game.h`: Important declarations and structs
- `userspace/vlibc/Makefile`
- `userpace/shell.c`: Added "game" command
- `userspace/vlibc/syscalls.arm.c`: Added syscalls for Framebuffer and keyboard and framebuffer console.
- `userspace/vlib/include/syscalls.h`: Added syscall numbers for framebuffer and keyboard and framebuffer console.
- `userspace/vlibc/font.c`: had to include because of global variable restrictions. But didn't work
- `userspace/vlibc/include/medieval_font.h`: had to include because of global variable restrictions. But didn't work

- `genromfs-0.5.2/genromfs.c`

- `kernel/include/syscalls/syscalls.h`: Added syscall numbers for framebuffer and keyboard and framebuffer console.
- `kernel/syscalls/syscalls.c`: Added syscalls for Framebuffer and keyboard and framebuffer console.
- `kernel/drivers/framebuffer/framebuffer.c`: Added gradient code. The first pice of code to if we can display colors.
- `kernel/include/drivers/framebuffer/framebuffer.h`
- `kernel/drivers/keyboard/ps2_keyboard.c`: Using VMWOS code as base and added the functions to facilitate the keyboard signal to play game.
- `kernel/drivers/keyboard/Makefile`
- `kernel/drivers/driver.c`: Added keyboard as default call
- `kernel/interrupts/interrupts.c`: Added custom interrupt check for keyboard. The standard interrupt wasn't working. I observed 4 bytes of displacement.
- `kernel/syscalls/times.c`: included sys_get_current_time(). This helped in adding non_blocking_delay(). An essential logic() that made keyboard work for the game.
- `kernel/drivers/framebuffer/framebuffer_console.c`: Added draw_text support that could be called by syscall from usersapce game to display scoring during the game play.

- Updated many makefiles to get new code compiled nad integrated correctly