
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
// http://www.applelogic.org/files/LANGCARDMAN.pdf

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

// ???
#pragma rodata-name(push,"CODE")
const char SOURCEFILE_1[] = {
  #embed "apple2-boot0.cfg"
  0
};
#pragma rodata-name(pop)
#pragma rodata-name(push,"LOWCODE")
const char SOURCEFILE_2[] = {
  #embed "apple2-boot0.cfg"
  0
};
#pragma rodata-name(pop)

// TODO: doesn't work yet
#pragma rodata-name(push,"LC")
const char IMAGE_COPY_1[] = {
  #embed "parrot-apple2.hires.bin"
  0
};
const char SOURCEFILE_3[] = {
  #embed "apple2-boot0.cfg"
  0
};
#pragma rodata-name(pop)


// peeks, pokes, and strobes
#define STROBE(addr) __asm__ ("sta %w", addr)
#define RSTROBE(addr) __asm__ ("bit %w", addr)

// clear screen and set graphics mode
void show_hgr0() {
  STROBE(0xc052); // turn off mixed-mode
  STROBE(0xc054); // page 1
  STROBE(0xc057); // hi-res
  STROBE(0xc050); // set graphics mode
}

void show_hgr1() {
  STROBE(0xc052); // turn off mixed-mode
  STROBE(0xc055); // page 2
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

int compare_with_lc(const char* main, const char* lc, unsigned int len) {
  int i;
  
//  RSTROBE(0xc080);
  for (i=0; i<len; i++) {
    if (*main++ != *lc++) return i+1;
  }
//  RSTROBE(0xc081);
//  RSTROBE(0xc081);
  return 0;
}

void wait_for_key() {
  puts("\r\nPress any key...");
  cgetc();
  puts("\r\n");
}

int main (void)
{
  // write protect
  RSTROBE(0xc080);
  // rebuild some ZP vars
  POKE(0x28,0); // BASL
  POKE(0x29,4);
  POKE(0x20,0); //left edge
  POKE(0X21,40);
  POKE(0x22,0); //top edge
  POKE(0x23,24); //bottom edge
  // stack pointer
  POKE(0x80,0);
  POKE(0x81,0xc0);
  while (1) {
    char i;
    text_mode();
    clrscr();
    revers(0);
    puts("Checking data...\n");
    printf("Check $2000/$4000: %d\n",
      compare_with_lc(TITLE_HGR1, TITLE_HGR2, 0x2000));
    printf("Check $2000/LC: %d\n",
      compare_with_lc(TITLE_HGR1, IMAGE_COPY_1, 0x2000));
    printf("Check CODE/LOWCODE: %d\n",
      compare_with_lc(SOURCEFILE_1, SOURCEFILE_2, strlen(SOURCEFILE_1)));
    printf("Check CODE/LC: %d\n",
      compare_with_lc(SOURCEFILE_1, SOURCEFILE_3, strlen(SOURCEFILE_1)));
    wait_for_key();
    for (i=0;i<0xc0; i++) {
      show_page_info(i);
    }
    for (i=0xd0;i!=0; i++) {
      show_page_info(i);
    }
    puts(SOURCEFILE_1);
    puts(SOURCEFILE_2);
    wait_for_key();
    show_hgr0();
    wait_for_key();
  }
  return EXIT_SUCCESS;
}
