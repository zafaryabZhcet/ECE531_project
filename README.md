# ECE531 Project

## Files that I Edited or Added

- `userspace/vlibc/game.c`: Keyboard interrupt not working in the infinite loop of game.c
- `userspace/vlib/include/game.h`
- `userspace/vlibc/Makefile`
- `userpace/shell.c`: Added "game command"
- `userspace/vlibc/syscalls.arm.c`: Added syscalls for Framebuffer and keyboard
- `userspace/vlib/include/syscalls.h`: Added syscall numbers for framebuffer and keyboard
- `userspace/vlibc/font.c`: had to include because of global variable restrictions. But didn't work
- `userspace/vlibc/include/medieval_font.h`: had to include because of global variable restrictions. But didn't work

- `genromfs-0.5.2/genromfs.c`

- `kernel/include/syscalls/syscalls.h`: Added syscall numbers for framebuffer and keyboard
- `kernel/syscalls/syscalls.c`: Added syscalls for Framebuffer and keyboard and framebuffer console.
- `kernel/drivers/framebuffer/framebuffer.c`: Added gradient code
- `kernel/include/drivers/framebuffer/framebuffer.h`
- `kernel/drivers/keyboard/ps2_keyboard.c`: Using VMWOS code for now
- `kernel/drivers/keyboard/Makefile`
- `kernel/drivers/driver.c`: Added keyboard as default call
- `kernel/interrupts/interrupts.c`: Added custom interrupt check for keyboard
- `kernel/syscalls/times.c`: included sys_get_current_time()
- `kernel/drivers/framebuffer/framebuffer_console.c`: Added draw_text support

- Updated many makefiles to get new code compiled nad integrated correctly