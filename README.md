0boot-cc65
=====

This is a demo of using [0boot](https://github.com/a2-4am/0boot/tree/master) and the CC65 toolchain to produce a DSK file using the linker. It loads a C program. Because there is no DOS and all the zeropage variables have been wiped by the bootloader, both the CC65 library functions and the ROM routines are a little confused.

Thanks to the #embed keyword it only works in dev builds of 8bitworkshop, for now.

[Open this project in 8bitworkshop](https://sehugg.github.io/8bitworkshop/redir.html?platform=apple2&githubURL=https%3A%2F%2Fgithub.com%2Fsehugg%2F0boot-cc65&file=testboot.c).
