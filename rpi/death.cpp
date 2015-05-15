#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include "GPIO.h"

using namespace std;

#define CheckResult(value1, op, value2, errorMessage) {\
    if (value1 op value2) {\
        perror(errorMessage);\
        exit(value1 op value2);\
    }\
}

/* Base constructor */
GPIO::GPIO() {
    /* Open /dev/mem for mmap to use. */
    mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    CheckResult(mmap_fd, ==, -1, 
            "Failed to open /dev/mem, try checking permissions.\n");

    mem_offset = GPIO_OFFSET;

    /* Call mmap at offset |mem_offset|. */
    map = mmap(
            NULL,
            BLOCK_SIZE,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            mmap_fd,
            mem_offset 
            );
    CheckResult((uintptr_t)map, ==, -1, "Failed mmap, exiting.\n"); 

    /* Volatile pointer assignment so that every reference to the memory results
     * in a direct memory access (as opposed to using stored values). */
    mem_addr = (volatile unsigned int *)map;
}

/* Base destructor */
GPIO::~GPIO() {
    int result;
    result = munmap(map, BLOCK_SIZE);
    CheckResult(result, ==, -1, "Failed call to munmap, exiting.\n");
    result = close(mmap_fd);
    CheckResult(result, ==, -1, "Failed call to close, exiting.\n");
}

int main(int argc, char** argv) {
    /* Make new GPIO object. */
    GPIO* gpio = new GPIO();

    /* Set gpio pins 4 & 17 as an output pin. */
    GPIO_SET_AS_OUTPUT(gpio, 4);
    GPIO_SET_AS_OUTPUT(gpio, 17);

    /* Set initial pin sates to spoof actual run. */
    GPIO_CLR(gpio, 4);
    GPIO_SET(gpio, 17);
    
    int input;
    scanf("%d", &input);

    /* Set pin 4 high to represent the signal coming through for main power
     * dying. */
    GPIO_SET(gpio, 4);
    
    /* Sleep for a set time to spoof having a capacitor inline. */
    usleep(input);

    /* Set pin 17 low to represent the pi dying. */
    GPIO_CLR(gpio, 17);
     
    return 0;
}
