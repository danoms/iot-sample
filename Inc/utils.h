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
  int apin;
};

void delay(unsigned int time);

#endif // UTILS_H_
