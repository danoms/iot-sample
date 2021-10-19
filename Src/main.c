#include "main.h"
#include "stm32f7xx.h"

int main(void) {
  struct led_t leds[LED_COUNT] = {
      LED(C, 7), LED(C, 6), LED(G, 6), LED(B, 4), LED(G, 7),
      LED(I, 0), LED(H, 6), LED(I, 3), LED(I, 2),
  };

  for (int i = 0; i < LED_COUNT; i++) {
    init_led(&leds[i]);
  }

  int go_up = 1;

  while (1) {
    led_train(leds, go_up);

    if (key_pressed())
      go_up ^= 1;

    return 0;
  }
}

int key_pressed() { return 0; }

void led_train(struct led_t leds[], int go_up) {
  static unsigned int i = 0;

  int led_current = i % LED_COUNT;

  leds[led_current].base->BSRR = leds[led_current].set_bit;
  delay(100);
  leds[led_current].base->BSRR = leds[led_current].reset_bit;

  if (go_up) {
    i++;
  } else {
    i--;
  }
}

void init_uart() {
  // enable clock for portC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  // set alternate function for PC6:D1, USART6:TX
  GPIOC->MODER &= ~(GPIO_MODER_MODER6);
  GPIOC->MODER |= GPIO_MODER_MODER6_1;

  // set alternate function for PC7:D0, USART6:RX
  GPIOC->MODER &= ~(GPIO_MODER_MODER7);
  GPIOC->MODER |= GPIO_MODER_MODER7_1;
}
