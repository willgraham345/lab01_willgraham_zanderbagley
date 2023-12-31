#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include "lab1.h"

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0	DT_GPIO_FLAGS(LED0_NODE, gpios)
#define LED1	DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1	DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1	DT_GPIO_FLAGS(LED1_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN0	0
#define FLAGS0	0
#define LED1	""
#define PIN1	0
#define FLAGS1	0
#endif

#define STACKSIZE 2000

struct k_thread coop_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);
int counter;
bool led_state;

void thread_entry(void)
{
    const struct device *dev;
    dev = device_get_binding(LED1);

	second_thread_setup(dev, PIN0, FLAGS0, &counter);

	struct k_timer t;
	k_timer_init(&t, NULL, NULL);

	while (1) {
		second_thread_iteration(dev, PIN0, led_state, &counter);
        
		k_timer_start(&t, K_MSEC(2000), K_NO_WAIT); // Library functions, no need to test
		k_timer_status_sync(&t); // Library functions, no need to test
	}
}

void main(void)
{
	const struct device *dev;

	dev = device_get_binding(LED1);

	led_state = main_thread_setup(dev, PIN1, FLAGS1);

    k_thread_create(&coop_thread,
                    coop_stack,
                    STACKSIZE,
                    (k_thread_entry_t) thread_entry,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

	while (1) {
        led_state = main_thread_loop(dev, PIN1, led_state);
        delay_ms(500);
    }
}
