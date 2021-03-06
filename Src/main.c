#include <stdio.h>
#include <stdlib.h>

#include "stm32f7xx.h"

#include "main.h"
#include "utils.h"

#define SYSCLK 16000000
#define LED_COUNT 9

void uart_init(int baudrate);
void led_init(struct led_t *led);
void key_init();
void led_train(struct led_t leds[], int go_up, int speed);

int main(void) {
  struct led_t leds[LED_COUNT] = {LED(G, 6), LED(B, 4),  LED(G, 7),
                                  LED(I, 0), LED(H, 6),  LED(I, 3),
                                  LED(I, 2), LED(A, 15), LED(B, 14)};

  for (int i = 0; i < LED_COUNT; i++) {
    led_init(&leds[i]);
  }

  uart_init(115200);
  key_init();

  LOG("-----------------\r\n");
  LOG("Debug on: led train\r\n");
  LOG("-----------------\r\n\r\n");

  int led_train_speed = uart_prompt("Insert led train speed: ");
  printf("Led train speed set to: %d\r\n", led_train_speed);

  int go_up = 1;
  while (1) {
    led_train(leds, go_up, led_train_speed);

    LOG("Key: %lx ", GPIOB->IDR);
    if (key_pressed(GPIOB, 8))
      go_up ^= 1;
  }

  return 0;
}

void led_train(struct led_t leds[], int go_up, int speed) {
  static unsigned int i = 0;

  int led_current = i % LED_COUNT;

  leds[led_current].base->BSRR = leds[led_current].set_bit;
  delay(speed);

#ifdef DEBUG
  char buffer[LED_COUNT + 1];
  int leds_state = led_pins_state(leds, LED_COUNT);
  int_to_bin(leds_state, buffer, LED_COUNT);
#endif
  LOG("LEDS %d: %s : %x\r\n", led_current, buffer, leds_state);

  leds[led_current].base->BSRR = leds[led_current].reset_bit;

  if (go_up) {
    i++;
  } else {
    i--;
  }
}

// Override the 'write' clib method to implement 'printf' over UART
// https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
int _write(int handle, char *data, int size) {
  UNUSED(handle); // suppress warning
  int count = size;
  while (count--) {
    while (!(USART6->ISR & USART_ISR_TXE)) {
    };
    USART6->TDR = *data++;
  }
  while (!(USART6->ISR & (USART_ISR_TC))) {
  };
  return size;
}

void uart_init(int baudrate) {
  // GPIOC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock for portC

  // TX
  // set alternate function for PC6:D1, USART6:TX
  GPIOC->MODER &= ~(GPIO_MODER_MODER6);
  GPIOC->MODER |= GPIO_MODER_MODER6_1;

  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT6);

  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6);
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR6_1;

  GPIOC->AFR[0] &= ~(GPIO_AFRL_AFRL6);
  GPIOC->AFR[0] |= GPIO_AFRL_AFRL6_3; // AF8

  // RX
  // set alternate function for PC7:D0, USART6:RX
  GPIOC->MODER &= ~(GPIO_MODER_MODER7);
  GPIOC->MODER |= GPIO_MODER_MODER7_1;

  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT7);

  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR7);
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR7_1;

  GPIOC->AFR[0] &= ~(GPIO_AFRL_AFRL7);
  GPIOC->AFR[0] |= GPIO_AFRL_AFRL7_3; // AF8

  // USART6
  RCC->APB2ENR |= RCC_APB2ENR_USART6EN; // enable clock for usart6

  USART6->CR1 &= ~(USART_CR1_M);    // 8-bit character length: M[1:0] = 00
  USART6->BRR = SYSCLK / baudrate;  // set baudrate
  USART6->CR2 &= ~(USART_CR2_STOP); // 1 stop bits: STOP[1:0]=00

  // usart enable, transfer enable, receive enable
  USART6->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
}

void led_init(struct led_t *led) {
  // enable clock for GPIO port
  SET_BIT(RCC->AHB1ENR, led->ahben_bit);

  // set port pins as output
  led->base->MODER &= ~(led->moder_mask);
  led->base->MODER |= led->moder_bit;
}

void key_init() {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8);
  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0; // pull-up
}
