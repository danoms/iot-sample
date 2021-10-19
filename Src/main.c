#include "stm32f7xx.h"

// Quick and dirty delay
static void delay(unsigned int time) {
  for (unsigned int i = 0; i < time; i++)
    for (volatile unsigned int j = 0; j < 2000; j++)
      ;
}

struct led_t {
  GPIO_TypeDef *base;
  int ahen;
  int moder_mask;
  int moder_bit;
  int set_bit;
  int reset_bit;
};

#define INIT_LED(port, pin)                                                    \
  (struct led_t) {                                                             \
    .base = GPIO##port, .ahen = RCC_AHB1ENR_GPIO##port##EN,                    \
    .moder_mask = GPIO_MODER_MODER##pin,                                       \
    .moder_bit = GPIO_MODER_MODER##pin##_0, .set_bit = GPIO_BSRR_BS_##pin,     \
    .reset_bit = GPIO_BSRR_BR_##pin                                            \
  }

void init(struct led_t *led) {
  SET_BIT(RCC->AHB1ENR, led->ahen);

  led->base->MODER &= ~(led->moder_mask);
  led->base->MODER |= led->moder_bit;
}

#define LED_COUNT 9

int main(void) {
  struct led_t leds[LED_COUNT] = {
      INIT_LED(C, 7), INIT_LED(C, 6), INIT_LED(G, 6),
      INIT_LED(B, 4), INIT_LED(G, 7), INIT_LED(I, 0),
      INIT_LED(H, 6), INIT_LED(I, 3), INIT_LED(I, 2),
  };

  for (int i = 0; i < LED_COUNT; i++) {
    init(&leds[i]);
  }

  int l = 0;
  int i = 0;

  while (1) {
    l = i % 3;
    // Reset the state of pin 13 to output low
    leds[l].base->BSRR = leds[l].set_bit;
    delay(100);
    leds[l].base->BSRR = leds[l].reset_bit;
    i++;
  }

  return 0;
}
