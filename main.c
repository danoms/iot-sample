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
  {                                                                            \
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
  // Turn on the GPIOC peripheral
  struct led_t leds[LED_COUNT];

  /* SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); */
  leds[0] = INIT_LED(C, 7);
  leds[1] = INIT_LED(C, 6);
  leds[2] = INIT_LED(G, 6);
  leds[3] = INIT_LED(B, 4);
  leds[4] = INIT_LED(G, 7);
  leds[5] = INIT_LED(I, 0);
  leds[6] = INIT_LED(H, 6);
  leds[7] = INIT_LED(I, 3);
  leds[8] = INIT_LED(I, 2);

  for (int i = 0; i < LED_COUNT; i++) {
    init(&leds[i]);
  }

  // Put the pin in genreal purpose output mode
  /* GPIOC->MODER &= ~(GPIO_MODER_MODER7); */
  /* GPIOC->MODER |= GPIO_MODER_MODER7_0; */

  /* GPIOC->MODER &= ~(GPIO_MODER_MODER6); */
  /* GPIOC->MODER |= GPIO_MODER_MODER6_0; */

  int l = 0;
  int i = 0;
  int goup = 1;

  while (1) {
    l = i % 3;
    // Reset the state of pin 13 to output low
    leds[l].base->BSRR = leds[l].set_bit;
    delay(100);
    leds[l].base->BSRR = leds[l].reset_bit;

    if (i == 30)
      goup = 0;
    if (i == 0)
      goup = 1;

    if (goup) {
      i++;
    } else {
      i--;
    }

    /* leds[0].base->BSRR = leds[0].reset_bit; */
    /* GPIOC->BSRR = GPIO_BSRR_BR_7; */
    /* GPIOC->BSRR = GPIO_BSRR_BR_6; */

    // Set the state of pin 13 to output high

    /* leds[0].base->BSRR = leds[0].set_bit; */
    /* GPIOC->BSRR = GPIO_BSRR_BS_7; */
    /* GPIOC->BSRR = GPIO_BSRR_BS_6; */
  }

  // Return 0 to satisfy compiler
  return 0;
}
