#include "led.h"

#include "hal.h"

int led_delay_ms = 500;

#define LED_DELAY_MS_MAX 3000
#define LED_DELAY_MS_MIN 50

void ledOff(void)
{
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
}

void ledOn(void)
{
    palSetPad(GPIOD, GPIOD_LED3);     /* Orange.  */
}

int ledGetDelay(void)
{
    return led_delay_ms;
}

int ledSetDelay(int new_delay)
{
    if(new_delay > LED_DELAY_MS_MIN && new_delay < LED_DELAY_MS_MAX)
    {
        led_delay_ms = new_delay;
        return led_delay_ms;
    }
    else
    {
        return -1;
    }
}
