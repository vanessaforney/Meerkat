#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define BCM2708_PERIPHERAL_BASE_OFFSET  0x20000000
#define GPIO_BASE_OFFSET                0x200000
#define GPIO_OFFSET (BCM2708_PERIPHERAL_BASE_OFFSET + GPIO_BASE_OFFSET) 

#define PAGE_SIZE 1024
#define BLOCK_SIZE (4 * PAGE_SIZE)

#define INPUT 0x0               /* State to set a pin as an input pin */
#define OUTPUT 0x1              /* State to set a pin as an output pin */

#define NUM_PERIPHERALS 10      /* Number of GPIO pins accessible in a 32 bit block */
#define GPIO_CLR_OFFSET 10      /* Offset to GPIO clear registers. */
#define GPIO_SET_OFFSET 7       /* Offset to GPIO set registers. */
#define CTRL_BITS 3             /* Number of control bits used to set the state of a pin */

/* Sets a pin g as an input pin. */
#define SET_AS_INPUT(g) *(mem_addr + ((g) / NUM_PERIPHERALS)) &= \
    ~((~INPUT) << ((g) % NUM_PERIPHERALS) * CTRL_BITS)

/* Sets a pin g as an output pin. */
#define SET_AS_OUTPUT(g) *(mem_addr + ((g) / NUM_PERIPHERALS)) &= \
    ~((~OUTPUT) << ((g) % NUM_PERIPHERALS) * CTRL_BITS)

/* Sets a pin g high. */
#define GPIO_SET(g) *(mem_addr + GPIO_SET_OFFSET) << (g)

/* Sets a pin g low. */
#define GPIO_CLR(g) *(mem_addr + GPIO_CLR_OFFSET) << (g)

class GPIO {
    public:
        unsigned long mem_offset;
        int mmap_fd;
        void *map;
        volatile unsigned int *mem_addr;

        /* Constructor */
        GPIO();

        /* Destructor */ 
        ~GPIO();
};

#endif
