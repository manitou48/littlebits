//
// cloudbit IO memory access   
//  LRADC ref ch 33


// Access from ARM Running Linux

#define GPIO_BASE 0x80050000

// LRADC  0x80050000

#define MASK23 0x0c
#define MAP89 0x76549810
#define TEMPSENS 0x8000

// offsets: pointer is long, offsets byte, hence /4
#define CH0 0x50/4
#define CH2 0x70/4
#define CH3 0x80/4
#define CTRL0 0x0/4
#define CTRL1 0x10/4
#define CTRL2 0x20/4
#define CTRL3 0x30/4
#define CTRL4 0x140/4

#define SET 4/4
#define CLR 8/4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;


// I/O access
volatile unsigned *gpio;



void setup_io();

void temp_init() {
  unsigned long v;

  *(gpio + CTRL2 + CLR) = TEMPSENS;   
  *(gpio + CTRL4) =  MAP89;   // ch 8 9 to 2 3
  v = *(gpio + CTRL4);    // LRADC channel map
  printf("%0x\n",v);
}

unsigned long temp_read() {
	unsigned long ch8,ch9;
	*(gpio + CTRL0 + SET) = MASK23;   // schedule
	while( *(gpio +CTRL0) & MASK23) ;  //  wait
	ch9 = *(gpio + CH3) & 0x7fffffff;
	ch8 = *(gpio + CH2) & 0x7fffffff;
	printf("%0x %0x\n",ch9,ch8);
	return (ch9-ch8);
}

int main(int argc, char **argv)
{ 
  int g,rep;
  unsigned long v;
  double k,f;

  // Set up gpi pointer for direct register access
  setup_io();
  
  v = *(gpio + CH0);    // LRADC0  cloudbit ADC in
  printf("%0x\n",v);

  temp_init();   // once 
  v = temp_read();
  k = v * .245;
  f = 1.8*(k-273) + 32;
  printf("%.1f K  %.1f F\n",k,f);
  sleep(1);
  v = temp_read();
  k = v * .245;
  f = 1.8*(k-273) + 32;
  printf("%.1f K  %.1f F\n",k,f);
  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io

