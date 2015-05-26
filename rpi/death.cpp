#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>
#include "GPIO.h"

#define START_SIG_PIN 4                                                                      
#define END_SIG_PIN   17   

using namespace std;

int main(int argc, char** argv) {
    /* Make new GPIO object. */
    GPIO* gpio = new GPIO();

    /* Set gpio pins START_SIG_PIN & END_SIG_PIN as an output pin. */
    GPIO_SET_AS_OUTPUT(gpio, START_SIG_PIN);
    GPIO_SET_AS_OUTPUT(gpio, END_SIG_PIN);

    /* Set initial pin state to spoof actual run. Note that during the
     * actual run this program still works because it sets the pin states
     * appropriately and then hangs, waiting for user input. */
    GPIO_CLR(gpio, START_SIG_PIN);
    GPIO_SET(gpio, END_SIG_PIN);
    
    int input;
    printf("Enter the desired amount of time between signals:");
    scanf("%d", &input);

    /* Set pin START_SIG_PIN high to represent the signal coming through for 
     * main power dying. */
    GPIO_SET(gpio, START_SIG_PIN);
    
    /* Sleep for a set time to spoof having a capacitor inline. */
    usleep(input);

    /* Set pin END_SIG_PIN low to represent the pi dying. */
    GPIO_CLR(gpio, END_SIG_PIN);
     
    return 0;
}
