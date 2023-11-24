
#define CFGFILE apple2-boot0.cfg
//#resource "apple2-boot0.cfg"

//#link "boot0.ca65"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include <apple2.h>

// https://mirrors.apple2.org.za/ground.icaen.uiowa.edu/Collections/1WSW/MEGA.PEEKS.AND.POKES.html
// https://6502disassembly.com/a2-rom/AutoF8ROM.html#SymCLRSCR

// text page during loading
#pragma rodata-name(push,"TXT0")
const char LOADER_TEXT[40] = "LOADING FROM DISK...";
#pragma rodata-name(pop)

// hgr1 page after loading
#pragma rodata-name(push,"HGR")
const char TITLE_HGR1[8192] = {
  #embed "parrot-apple2.hires.bin"
};
#pragma rodata-name(pop)

// hgr2 page after loading
#pragma rodata-name(push,"HGR2")
const char TITLE_HGR2[8192] = {
  #embed "parrot-apple2.hires.bin"
};
#pragma rodata-name(pop)

// hey, why not one more...
#pragma rodata-name(push,"RODATA")
const char TITLE_RODATA[8192] = {
  #embed "parrot-apple2.hires.bin"
};
#pragma rodata-name(pop)

// ???
#pragma rodata-name(push,"LOWCODE")
const char SOURCEFILE_1[] = {
  #embed "testboot.c"
  0
};
const char SOURCEFILE_2[] = {
  #embed "apple2-boot0.cfg"
  0
};
#pragma rodata-name(pop)

// TODO: doesn't work yet
#pragma rodata-name(push,"LC")
const char SOURCEFILE_3[] = {
  #embed "testboot.c"
  0
};
#pragma rodata-name(pop)


// peeks, pokes, and strobes
#define STROBE(addr) __asm__ ("sta %w", addr)

// clear screen and set graphics mode
void show_hgr0() {
  STROBE(0xc052); // turn off mixed-mode
  STROBE(0xc054); // page 1
  STROBE(0xc057); // hi-res
  STROBE(0xc050); // set graphics mode
}

void text_mode() {
  STROBE(0xc051); // set text mode
}

void show_page_info(char page) {
  int i;
  int sum = 0;
  int address = (int)page << 8;
  for (i=0; i<256; i++) {
    sum += PEEK(address++);
  }
  printf("Page %02x: sum = %04x\n", page, sum);
}

int main (void)
{
  // rebuild some ZP vars
  POKE(32,0); //left edge
  POKE(33,40);
  POKE(34,0); //top edge
  POKE(35,24); //bottom edge
  while (1) {
    char i;
    show_hgr0();
    cgetc();
    text_mode();
    clrscr();
    revers(0);
    for (i=0;i<0xc0; i++) {
      show_page_info(i);
    }
    for (i=0xd0;i!=0; i++) {
      show_page_info(i);
    }
    puts(SOURCEFILE_1);
    puts(SOURCEFILE_2);
    puts(SOURCEFILE_3);
  }
  return EXIT_SUCCESS;
}
