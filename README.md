0boot-cc65
=====

This is a demo of using [0boot](https://github.com/a2-4am/0boot/tree/master) and the CC65 toolchain to produce a DSK file using the linker. It loads and runs a C program. Because there is no DOS present and all the zeropage variables have been wiped by the bootloader, both the CC65 library functions and the ROM routines are a little confused, and we have to fix-up memory a bit. This could be fixed with a different crt0 and stdlib functions, or just by using CA65 directly. If you avoid all ROM calls, you can use all 256 bytes of zero page.

Thanks to the #embed keyword the demo program only works in [dev builds](https://sehugg.github.io/8bitworkshop/?platform=apple2) of 8bitworkshop, for now.
