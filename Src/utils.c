#include "utils.h"

// Quick and dirty delay
void delay(unsigned int time) {
  for (unsigned int i = 0; i < time; i++)
    for (volatile unsigned int j = 0; j < 2000; j++)
      ;
}

void led_init(struct led_t *led) {
  // enable clock for GPIO port
  SET_BIT(RCC->AHB1ENR, led->ahben_bit);

  // set port pins as output
  led->base->MODER &= ~(led->moder_mask);
  led->base->MODER |= led->moder_bit;
}
