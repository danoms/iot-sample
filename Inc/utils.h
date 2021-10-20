#include "stm32f7xx.h"

#ifndef UTILS_H_
#define UTILS_H_

struct led_t {
  GPIO_TypeDef *base;
  int ahben_bit;
  int moder_mask;
  int moder_bit;
  int set_bit;
  int reset_bit;
};

void delay(unsigned int time);
void led_init(struct led_t *led);

#endif // UTILS_H_
