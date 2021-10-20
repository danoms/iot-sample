#include "stm32f7xx.h"
#include <stdio.h>
#include <stdlib.h>

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
void int_to_bin(int num, char *buffer, int size);
int key_pressed(GPIO_TypeDef *base, int pin);
int uart_prompt(char *prompt);
int led_pins_state(struct led_t leds[], int size);

#endif // UTILS_H_
