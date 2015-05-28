#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include <sys/time.h>
#include "GPIO.h"

#define MICROSEC_CONV 1000000.0
#define START_SIG_PIN 4
#define END_SIG_PIN   17

using namespace std;

int main(int argc, char** argv) {
    struct timeval start;
    struct timeval end;
    double startTime;
    double endTime;

    /* Make new GPIO object. */
    GPIO* gpio = new GPIO();

    /* Sets gpio pin START_SIG_PIN and END_SIG_PIN to input. */
    GPIO_SET_AS_INPUT(gpio, START_SIG_PIN);
    GPIO_SET_AS_INPUT(gpio, END_SIG_PIN);

    /* Poll until signal comes presenting a signal that main power died. */
    while ((GPIO_READ(gpio, START_SIG_PIN)) == 0) {
    } 

    /* Start timer. */
    gettimeofday(&start, NULL);

    /* Loop until the signal on END_SIG_PIN goes low, signifying that the pi
     * is dead. */
    while (GPIO_READ(gpio, END_SIG_PIN)) {
    }

    /* Stop timer. */
    gettimeofday(&end, NULL);

    /* Calculate total elapsed time. */
    startTime  = start.tv_sec + (start.tv_usec / MICROSEC_CONV);
    endTime  = end.tv_sec + (end.tv_usec / MICROSEC_CONV);
    printf("Time to death: %.6lf\n", endTime - startTime);

    return 0;
}
