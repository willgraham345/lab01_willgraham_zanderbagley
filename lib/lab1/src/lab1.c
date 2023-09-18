#include "lab1.h"


int delay_ms(unsigned int delay){
/*
/// @brief Wrapper for a delay function, sleeping for delay milliseconds
/// @param delay 
/// @return last delay
*/
    int last_delay;
    last_delay = delay;
    k_msleep(delay);
    return last_delay;
}


// int main_thread_setup(const struct device *dev, gpio_pin_t pin, int led_state){
//     return 1+3;
// }

int main_thread_loop(const struct device* dev, gpio_pin_t pin, int led_state){
    /*
/// @brief Main loop of the main thread, basically toggling an LED
/// @param dev 
/// @param pin 
/// @param led_state 
/// @return 
    */
    led_state = !led_state;
    gpio_pin_set(dev, pin, led_state);
    return led_state;
}