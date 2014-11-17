/*
 *  cloudbit DAC mmap  ch 29  AUDIOOUT 0x80048000
 * based on devmem2.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define AUDIOOUT 0x80048000
#define ADUIOOUT_DATA 0x00f0

int main(int argc, char **argv) {
    int fd;
    int16_t dac16;                   // 2 16-bit values 2s complement
    double dacv, dacp;
    void *map_base, *virt_addr; 
	unsigned long read_result, writeval;
	off_t target;
	int access_type = 'w';
	

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    
    /* Map one page */
    target = AUDIOOUT + ADUIOOUT_DATA;
    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) FATAL;
    
    virt_addr = map_base + (target & MAP_MASK);
    read_result = *((unsigned long *) virt_addr);
    dac16 = *((int16_t *) virt_addr);
    dacv = 2.5 + .000076*dac16;   // voltage
    dacp = 100.*dacv/5.0;            // percent
    printf("DAC %0x %d  %.1fv %.0f \n",read_result,dac16,dacv,dacp);
    fflush(stdout);
    close(fd);
    return 0;
}

