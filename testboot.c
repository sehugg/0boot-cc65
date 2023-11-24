
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
const char TITLE_HGR0[8192] = {
  #embed "parrot-apple2.hires.bin"
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

int main (void)
{
  /*
  // set up stack
  asm("lda #0");
  asm("sta sp");
  asm("lda #$c0");
  asm("sta sp+1");
  */
  show_hgr0();
  cgetc();
  text_mode();
//  memset((void*)0x400, 0x21, 40);
  POKE(32,0); //left edge
  POKE(33,40);
  POKE(34,0); //top edge
  POKE(35,24); //bottom edge
  //asm("jsr $fb2f");
  //asm("jsr $fe93");
  //asm("jsr $fe89");
  clrscr();
  revers(0);
  cputs("Hello!\nPress a key to reboot (?)");
//  printf("\nHello! Press a key to reboot...\n");
  while (1) {
    char ch = cgetc();
    printf("%3d ($%2x) = %c\n", ch, ch, ch);
  }
  return EXIT_SUCCESS;
}
