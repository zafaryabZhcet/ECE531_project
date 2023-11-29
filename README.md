# ECE531 Project

## Files that I Edited or Added

- `userspace>vlibc>game.c`
- `userspace>vlib>include>game.h`
- `userspace>vlibc>Makefile`
- `userpace>shell.c`
- `userspace>vlibc>syscalls.arm.c`: Added syscalls for Framebuffer and keyboard
- `userspace>vlib>include>syscalls.h`: Added syscall numbers for framebuffer and keyboard

- `genromfs-0.5.2>genromfs.c`

- `kernel>include>syscalls>syscalls.h`: Added syscall numbers for framebuffer and keyboard
- `kernel>syscalls>syscalls.c`: Added syscalls for Framebuffer and keyboard
- `kernel>drivers>framebuffer>framebuffer.c`: Added gradient code
- `kernel>include>drivers>framebuffer>framebuffer.h`
- `kernel>drivers>keyboard>ps2_keyboard.c`: Using VMWOS code for now
- `kernel>drivers>keyboard>Makefile`
- `kernel>drivers>driver.c`: Added keyboard as default call
- `kernel>interrupts>interrupts.c`: Added custom interrupt check for keyboard

