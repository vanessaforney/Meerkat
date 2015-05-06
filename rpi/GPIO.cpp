#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include "GPIO.h"

using namespace std;

#define CheckExpression(expression, errorMessage) {\
    if (expression) {\
        perror(errorMessage);\
        exit(expression);\
    }\
}

/* Base constructor */
GPIO::GPIO() {
    /* Open /dev/mem for mmap to use. */
    mmap_fd = open("/dev/mem", O_RDWR | O_SYNC);
    CheckExpression(mmap_fd != -1, 
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
    CheckExpression((uintptr_t)map != -1, "Failed mmap, exiting.\n"); 
}

/* Base destructor */
GPIO::~GPIO() {
    munmap(map, BLOCK_SIZE);
    close(mmap_fd);
}

int main(int argc, char** argv) {
    GPIO* gpio = new GPIO();
    return 0;
}
