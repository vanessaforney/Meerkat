#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include <sys/time.h>
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
    struct timeval start;
    struct timeval end;

    /* Make new GPIO object. */
    GPIO* gpio = new GPIO();

    /* Sets gpio pin 4 and 17 to input. */
    GPIO_SET_AS_INPUT(gpio, 4);
    GPIO_SET_AS_INPUT(gpio, 17);

    /* Poll until signal comes presenting a signal that main power died. */
    while ((GPIO_READ(gpio, 4)) == 0) {
    } 

    /* Start timer. */
    gettimeofday(&start, NULL);

    printf("%d", (GPIO_READ(gpio, 17)));
    while (GPIO_READ(gpio, 17)) {
    }

    /* Stop timer. */
    gettimeofday(&end, NULL);

    /* Calculate total elapsed time. */
    double t1=start.tv_sec + (start.tv_usec/1000000.0);
    double t2=end.tv_sec + (end.tv_usec/1000000.0);
    printf("Time to death: %.6lf\n", t2-t1);

    return 0;
}
