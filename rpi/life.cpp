#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include <sys/time.h>
#include "GPIO.h"

#define MICROSEC_CONV 1000000.0

using namespace std;

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
    double t1 = start.tv_sec + (start.tv_usec / MICROSEC_CONV);
    double t2 = end.tv_sec + (end.tv_usec / MICROSEC_CONV);
    printf("Time to death: %.6lf\n", t2-t1);

    return 0;
}
