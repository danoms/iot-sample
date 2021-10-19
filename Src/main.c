#include "stm32f7xx.h"
#if __cplusplus > 199711L
#define register // Deprecated in C++11.
#endif           // #if __cplusplus > 199711L

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
  // Turn on the GPIOC peripheral
  struct led_t leds[LED_COUNT] = {
      INIT_LED(C, 7), INIT_LED(C, 6), INIT_LED(G, 6),
      INIT_LED(B, 4), INIT_LED(G, 7), INIT_LED(I, 0),
      INIT_LED(H, 6), INIT_LED(I, 3), INIT_LED(I, 2),
  };

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
  while (1) {
    l = i % 3;
    // Reset the state of pin 13 to output low
    leds[l].base->BSRR = leds[l].set_bit;
    delay(100);
    leds[l].base->BSRR = leds[l].reset_bit;
    i++;

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
