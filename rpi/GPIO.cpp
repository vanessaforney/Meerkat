#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include "GPIO.h"

using namespace std;

#define CheckResult(expression, errorMessage) {\
    if (expression) {\
        perror(errorMessage);\
        exit(expression);\
    }\
}

/* Base constructor */
GPIO::GPIO() {
    /* Open /dev/mem for mmap to use. */
    mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    CheckResult(mmap_fd == -1, 
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
    CheckResult((uintptr_t)map == -1, "Failed mmap, exiting.\n"); 

    /* Volatile pointer assignment so that every reference to the memory results
     * in a direct memory access (as opposed to using stored values). */
    mem_addr = (volatile unsigned int *)map;
}

/* Base destructor */
GPIO::~GPIO() {
    int result;
    result = munmap(map, BLOCK_SIZE);
    CheckResult(result == -1, "Failed call to munmap, exiting.\n");
    result = close(mmap_fd);
    CheckResult(result == -1, "Failed call to close, exiting.\n");
}
