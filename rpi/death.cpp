#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include "GPIO.h"

using namespace std;

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
