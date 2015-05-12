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

#define NUM_GPIOS 10            /* Number of GPIO pins accessible in a 32 bit block */
#define GPIO_CLR_OFFSET 10      /* Offset to GPIO clear registers. */
#define GPIO_SET_OFFSET 7       /* Offset to GPIO set registers. */
#define GPIO_VALUE_OFFSET 13    /* Offset to GPIO value registers. */
#define CTRL_BITS 3             /* Number of control bits used to set the state of a pin */

#define PIN_INPUT_MASK 7        
#define PIN_OUTPUT_MASK 1

/* Sets a pin g as an input pin. */
#define GPIO_SET_AS_INPUT(gpio, g) *(gpio->mem_addr + ((g) / NUM_GPIOS)) &= \
    ~(PIN_INPUT_MASK << (((g) % NUM_GPIOS) * CTRL_BITS))

/* Sets a pin g as an output pin. */
#define GPIO_SET_AS_OUTPUT(gpio, g) {\
    GPIO_SET_AS_INPUT(gpio,g);\
    *(gpio->mem_addr + ((g)/NUM_GPIOS)) |= \
        (PIN_OUTPUT_MASK << (((g) % NUM_GPIOS) * CTRL_BITS));\
}
 
/* Sets a pin g high. */
#define GPIO_SET(gpio, g) *(gpio->mem_addr + GPIO_SET_OFFSET) = (1 << (g))

/* Sets a pin g low. */
#define GPIO_CLR(gpio, g) *(gpio->mem_addr + GPIO_CLR_OFFSET) = (1 << (g))

/* Returns the value of pin g. */
#define GPIO_READ(gpio, g)  *(gpio->mem_addr + GPIO_VALUE_OFFSET) &= (1 << (g))

class GPIO {
    public:
        unsigned long mem_offset;           /* Offset physical memory is at. */
        int mmap_fd;                        /* Fd for mmap to access /dev/mem. */
        void *map;                          /* Pointer for the mmap map. */
        volatile unsigned int *mem_addr;    /* Volatile pointer for ensured memory access to map. */

        /* Constructor */
        GPIO();

        /* Destructor */ 
        ~GPIO();
};

#endif
