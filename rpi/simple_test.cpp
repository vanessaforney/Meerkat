#include "GPIO.h"

int main(int argc, char** argv) {
    int current, last = -1;
    GPIO* gpio = new GPIO();
    GPIO_SET_AS_INPUT(gpio, 4);

    while (true) {
    	current = (GPIO_READ(gpio, 4));
    	if (last != current) {
    		cout << "4: " << current << endl;
    		last = current;
    	}
    } 
}