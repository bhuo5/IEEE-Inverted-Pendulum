#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define ENCODER_BLACK 0
#define ENCODER_WHITE 1
#define PPR 1024.0  // advertised encoder pulses per revolution
#define DPR 360.0   // degrees in one revolution

volatile double angle = 0.00;
volatile int new_msg = 0;

void encoder_callback(uint gpio, uint32_t event)
{
    if (gpio_get(ENCODER_BLACK) != gpio_get(ENCODER_WHITE))
    {
        angle += DPR / PPR;
        new_msg = 1;
    }
    else
    {
        angle -= DPR / PPR;
        new_msg = 1;
    }
}

void setup_encoder()
{
    gpio_init(ENCODER_BLACK);
    gpio_set_dir(ENCODER_BLACK, GPIO_IN);
    gpio_pull_down(ENCODER_BLACK);

    gpio_init(ENCODER_WHITE);
    gpio_set_dir(ENCODER_WHITE, GPIO_IN);
    gpio_pull_down(ENCODER_BLACK);

    gpio_set_irq_enabled(ENCODER_BLACK, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_callback(encoder_callback);
}

int main()
{
    stdio_init_all();
    cyw43_arch_init();
    setup_encoder();

    while (1)
    {
        if (new_msg)
        {
            printf("%lf\n", angle);
            new_msg = 0;
        }

        tight_loop_contents();
    }
}